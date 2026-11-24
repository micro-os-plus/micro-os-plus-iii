/*
 * DO NOT EDIT! Automatically generated from template file:
 * npm-packages-helper/templates/common/.github/skills/_micro-os-plus/design-review/scripts/check-headers-liquid.mjs
 *
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2026 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

// ----------------------------------------------------------------------------

// Check that each public header compiles on its own, i.e. that it
// includes everything it uses, instead of relying on the order in which
// the umbrella header includes the others.
//
// Usage:
//   node check-headers.mjs <scratch-folder> [header-folder...] [-- <flag>...]
//
// Run it from the repository root. The compiler is taken from the CXX
// environment variable (default `c++`). Each `.h` file in the given
// folders (relative to `include/`; by default every `.h` file under
// `include/`, except those in the `inlines/` folders) is included alone
// in a small translation unit, compiled with `-std=c++20 -fsyntax-only`,
// and with the flags given after `--` (for example the `-D` options that
// enable the library). The exit code is 1 if any header fails.

import { spawnSync } from 'child_process'
import fs from 'fs'
import path from 'path'

// ----------------------------------------------------------------------------

const args = process.argv.slice(2)
const separatorIndex = args.indexOf('--')
const positionalArgs =
  separatorIndex >= 0 ? args.slice(0, separatorIndex) : args
const extraFlags = separatorIndex >= 0 ? args.slice(separatorIndex + 1) : []

const [scratchFolderPath, ...headerFolders] = positionalArgs
if (!scratchFolderPath) {
  console.error(
    'Usage: node check-headers.mjs <scratch-folder> [header-folder...] ' +
      '[-- <flag>...]'
  )
  process.exit(2)
}

const compiler = process.env.CXX || 'c++'
const includeFolderPath = 'include'

fs.mkdirSync(scratchFolderPath, { recursive: true })
const unitFilePath = path.join(scratchFolderPath, 'check-header.cpp')

// ----------------------------------------------------------------------------

// Returns the paths, relative to `include/`, of the `.h` files in the
// folder, recursively, except those in the `inlines/` folders.
const listHeaders = (relativeFolderPath) => {
  const entries = fs.readdirSync(
    path.join(includeFolderPath, relativeFolderPath),
    { withFileTypes: true }
  )
  const result = []
  for (const entry of entries) {
    const relativePath = path.join(relativeFolderPath, entry.name)
    if (entry.isDirectory()) {
      if (entry.name !== 'inlines') {
        result.push(...listHeaders(relativePath))
      }
    } else if (entry.name.endsWith('.h')) {
      result.push(relativePath)
    }
  }
  return result
}

const headers = (headerFolders.length > 0 ? headerFolders : [''])
  .flatMap((folder) => listHeaders(folder))
  .sort()

let failedCount = 0

for (const includePath of headers) {
  fs.writeFileSync(
    unitFilePath,
    `#include "${includePath}"\n\nint\nmain (void)\n{\n  return 0;\n}\n`
  )

  const result = spawnSync(
    compiler,
    [
      '-std=c++20',
      '-fsyntax-only',
      ...extraFlags,
      `-I${includeFolderPath}`,
      unitFilePath,
    ],
    { encoding: 'utf8' }
  )

  if (result.error) {
    console.error(`Error: cannot run ${compiler}: ${result.error.message}`)
    process.exit(2)
  }

  const errorsCount = (result.stderr.match(/error:/g) || []).length
  if (result.status === 0) {
    console.log(`ok      ${includePath}`)
  } else {
    failedCount++
    console.log(`FAILED  ${includePath} (${errorsCount} errors)`)
    const firstError = result.stderr
      .split('\n')
      .find((line) => line.includes('error:'))
    if (firstError) {
      console.log(`        ${firstError.trim()}`)
    }
  }
}

console.log(`${headers.length} headers, ${failedCount} failed`)
process.exit(failedCount > 0 ? 1 : 0)

// ----------------------------------------------------------------------------
