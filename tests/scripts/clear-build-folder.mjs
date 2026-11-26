/*
 * DO NOT EDIT! Automatically generated from template file:
 * build-helper/templates/common/_micro-os-plus/tests/scripts/clear-build-folder-liquid.mjs
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

import fs from 'fs'
import path from 'path'

const buildFolderPath = process.argv[2]
const keepDirs = ['xpacks']

if (!buildFolderPath) {
  console.error('Error: build folder argument is required.')
  console.error('Usage: node clear-build.mjs <build folder>')
  process.exit(1)
}

if (!fs.existsSync(buildFolderPath)) {
  console.error(`Error: '${buildFolderPath}' does not exist.`)
  process.exit(1)
}

if (!fs.statSync(buildFolderPath).isDirectory()) {
  console.error(`Error: '${buildFolderPath}' is not a folder.`)
  process.exit(1)
}

const entries = fs.readdirSync(buildFolderPath, { withFileTypes: true })

for (const entry of entries) {
  if (keepDirs.includes(entry.name)) {
    continue
  }

  const fullPath = path.join(buildFolderPath, entry.name)
  fs.rmSync(fullPath, { recursive: true, force: true })
  console.log(`Removed: ${fullPath}${entry.isDirectory() ? '/' : ''}`)
}

console.log('Done.')

// ----------------------------------------------------------------------------
