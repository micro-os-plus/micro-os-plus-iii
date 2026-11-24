/*
 * DO NOT EDIT! Automatically generated from template file:
 * npm-packages-helper/templates/common/.github/skills/_micro-os-plus/design-review/scripts/strip-comments-liquid.mjs
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

// Write copies of the C/C++ sources without comments and without blank
// lines, each remaining line prefixed by its original line number, so
// that the code can be reviewed without the documentation, while the
// findings can still cite the original line numbers.
//
// Usage:
//   node strip-comments.mjs <output-folder> [path...]
//
// The paths are files or folders, relative to the current folder; the
// default is `include` and `src`. Only `.h`, `.hpp`, `.c`, and `.cpp`
// files are processed. The output keeps the relative paths, e.g.
// `<output-folder>/include/micro-os-plus/<name>.h`.
//
// String and character literals (including raw strings) are preserved;
// a quote preceded by a digit or a letter is treated as a digit
// separator (`1'000`), not as the start of a character literal.

import fs from 'fs'
import path from 'path'

// ----------------------------------------------------------------------------

const outputFolderPath = process.argv[2]
if (!outputFolderPath) {
  console.error('Usage: node strip-comments.mjs <output-folder> [path...]')
  process.exit(2)
}

const inputPaths =
  process.argv.length > 3 ? process.argv.slice(3) : ['include', 'src']

const extensions = new Set(['.h', '.hpp', '.c', '.cpp'])

// ----------------------------------------------------------------------------

/**
 * Collect the source files under a path, recursively.
 *
 * @param {string} inputPath A file or a folder.
 * @returns {string[]} The source file paths, sorted.
 */
function collectFiles(inputPath) {
  const stat = fs.statSync(inputPath)
  if (stat.isFile()) {
    return extensions.has(path.extname(inputPath)) ? [inputPath] : []
  }

  const files = []
  for (const entry of fs.readdirSync(inputPath).sort()) {
    files.push(...collectFiles(path.join(inputPath, entry)))
  }
  return files
}

/**
 * Replace the comments with spaces, keeping the line breaks, so that
 * the line numbers do not change.
 *
 * @param {string} text The source text.
 * @returns {string} The text without comments.
 */
function stripComments(text) {
  let result = ''
  let i = 0
  const n = text.length

  while (i < n) {
    const c = text[i]

    // Line comment.
    if (c === '/' && text[i + 1] === '/') {
      while (i < n && text[i] !== '\n') {
        i++
      }
      continue
    }

    // Block comment; keep its line breaks.
    if (c === '/' && text[i + 1] === '*') {
      const end = text.indexOf('*/', i + 2)
      const stop = end < 0 ? n : end + 2
      result += text.slice(i, stop).replace(/[^\n]/g, '')
      i = stop
      continue
    }

    // Raw string literal: R"delimiter( ... )delimiter".
    if (c === 'R' && text[i + 1] === '"' && !/\w/.test(text[i - 1] ?? '')) {
      const open = text.indexOf('(', i + 2)
      if (open >= 0) {
        const delimiter = text.slice(i + 2, open)
        const close = text.indexOf(`)${delimiter}"`, open + 1)
        const stop = close < 0 ? n : close + delimiter.length + 2
        result += text.slice(i, stop)
        i = stop
        continue
      }
    }

    // String or character literal; a quote after a digit or a letter
    // is a digit separator.
    if (c === '"' || (c === "'" && !/[0-9A-Za-z]/.test(text[i - 1] ?? ''))) {
      let j = i + 1
      while (j < n && text[j] !== c && text[j] !== '\n') {
        j += text[j] === '\\' ? 2 : 1
      }
      result += text.slice(i, j + 1)
      i = j + 1
      continue
    }

    result += c
    i++
  }

  return result
}

// ----------------------------------------------------------------------------

let filesCount = 0
let linesCount = 0

for (const inputPath of inputPaths) {
  for (const filePath of collectFiles(inputPath)) {
    const stripped = stripComments(fs.readFileSync(filePath, 'utf8'))

    const lines = []
    stripped.split('\n').forEach((line, index) => {
      if (line.trim() !== '') {
        lines.push(`${String(index + 1).padStart(5)} ${line.trimEnd()}`)
      }
    })

    const outputFilePath = path.join(outputFolderPath, filePath)
    fs.mkdirSync(path.dirname(outputFilePath), { recursive: true })
    fs.writeFileSync(outputFilePath, lines.join('\n') + '\n')

    filesCount++
    linesCount += lines.length
  }
}

console.log(
  `${filesCount} files, ${linesCount} lines of code, written to ` +
    `${outputFolderPath}`
)

// ----------------------------------------------------------------------------
