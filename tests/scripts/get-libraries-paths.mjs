/*
 * DO NOT EDIT! Automatically generated from template file:
 * build-helper/templates/common/_micro-os-plus/tests/scripts/get-libraries-paths-liquid.mjs
 *
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2024-2026 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

// ----------------------------------------------------------------------------

import { execFileSync } from 'child_process'
import fs from 'fs'
import path from 'path'
import { globSync } from 'glob'

// ----------------------------------------------------------------------------

const compilerArgs = process.argv.slice(2)

if (compilerArgs.length < 1) {
  console.error(
    'Usage: node get-libraries-paths.mjs clang++|g++ [options...]'
  )
  process.exit(1)
}

// Resolves symlinks like Python's os.path.realpath(), without requiring
// the final path segment to exist (unlike fs.realpathSync()).
const tolerantRealpath = (inputPath) => {
  try {
    return fs.realpathSync(inputPath)
  } catch {
    return path.resolve(inputPath)
  }
}

const runCompiler = (extraArgs) =>
  execFileSync(compilerArgs[0], [...compilerArgs.slice(1), ...extraArgs], {
    encoding: 'utf8',
  }).trim()

const compilerName = path.basename(compilerArgs[0])

let libsPath = ''

if (/clang/.test(compilerName)) {
  if (process.platform === 'linux') {
    const runtimePath = runCompiler(['-print-runtime-dir'])
    const libcppPath = runCompiler(['-print-file-name=libc++.so'])
    libsPath = `${path.dirname(tolerantRealpath(libcppPath))}:${runtimePath}`
  } else if (process.platform === 'darwin') {
    const cxxAbsolutePath = tolerantRealpath(
      execFileSync('which', [compilerArgs[0]], { encoding: 'utf8' }).trim()
    )
    const libAbsolutePath = path.join(
      path.dirname(path.dirname(cxxAbsolutePath)),
      'lib'
    )
    if (
      fs.existsSync(libAbsolutePath) &&
      fs.statSync(libAbsolutePath).isDirectory()
    ) {
      const matches = globSync('**/libc++.dylib', {
        cwd: libAbsolutePath,
        absolute: true,
      })
      if (matches.length > 0) {
        libsPath = `${path.dirname(matches[0])}:`
      }
    }
    libsPath += path.dirname(runCompiler(['-print-libgcc-file-name']))
  } else {
    console.error('Unsupported platform')
    process.exit(1)
  }
} else if (/g[c+][c+]/.test(compilerName)) {
  // On macOS all libraries are in lib.
  // On Linux x64 they are in lib64/lib32.
  // On Linux arm64/arm they are in lib.
  const libstdcppPath = runCompiler(['-print-file-name=libstdc++.a'])
  if (process.platform === 'linux' || process.platform === 'darwin') {
    libsPath = path.dirname(tolerantRealpath(libstdcppPath))
  } else {
    console.error('Unsupported platform')
    process.exit(1)
  }
}

process.stdout.write(libsPath)

// ----------------------------------------------------------------------------
