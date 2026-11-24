/*
 * DO NOT EDIT! Automatically generated from template file:
 * npm-packages-helper/templates/common/.github/skills/_micro-os-plus/doxygen-review/scripts/check-doxygen-style-liquid.mjs
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

// Check the Doxygen comment blocks against the project documentation rules
// that Doxygen itself does not enforce: line length, tabs, malformed or
// misaligned comment lines, British spelling, contractions, preferred
// terminology, and `@details` paragraphs that repeat the `@brief`.
//
// Usage:
//   node check-doxygen-style.mjs [file-or-folder...]
//
// Without arguments, all C/C++ files in `include/` and `src/` are checked.
// The exit code is 1 if any problem is found.

import fs from 'fs'
import path from 'path'
import { globSync } from 'glob'

// ----------------------------------------------------------------------------

// Matches the `.clang-format` column limit.
const maxLineLength = 79

const americanSpellings = new RegExp(
  '\\b(' +
    [
      'initializ\\w*',
      'behavior\\w*',
      'color\\w*',
      'organiz\\w*',
      'analyz\\w*',
      'favor\\w*',
      'recogniz\\w*',
      'serializ\\w*',
      'normaliz\\w*',
      'minimiz\\w*',
      'maximiz\\w*',
      'customiz\\w*',
      'specializ\\w*',
      'realiz\\w*',
      'optimiz\\w*',
      'utiliz\\w*',
      'emphasiz\\w*',
      'summariz\\w*',
      'categoriz\\w*',
      'prioritiz\\w*',
      'finaliz\\w*',
      'synchroniz\\w*',
      'center\\w*',
      'canceled',
      'labeled',
      'modeled',
      'signaled',
      'defense',
      'gray',
    ].join('|') +
    ')\\b',
  'i'
)

// British English uses "program" for computer programs; "programme" is
// almost always a mistake in this context.
const wrongWords = /\b(programme\w*|director(y|ies))\b/i

const contractions = new RegExp(
  "\\b(" +
    [
      "don't",
      "doesn't",
      "isn't",
      "aren't",
      "can't",
      "won't",
      "wouldn't",
      "shouldn't",
      "couldn't",
      "didn't",
      "hasn't",
      "haven't",
      "wasn't",
      "weren't",
      "it's",
      "that's",
      "there's",
      "we're",
      "you're",
      "they're",
      "let's",
    ].join('|') +
    ')\\b',
  'i'
)

// Identifiers and code spans are not prose; skip them when checking words.
function prose(line) {
  return line.replace(/`[^`]*`/g, '').replace(/\b\w+_\w*\b/g, '')
}

function normalise(text) {
  return text.replace(/[\s*]+/g, ' ').trim().toLowerCase()
}

// ----------------------------------------------------------------------------

const args = process.argv.slice(2)
const patterns =
  args.length > 0
    ? args.map((arg) =>
        fs.existsSync(arg) && fs.statSync(arg).isDirectory()
          ? path.join(arg, '**/*.{h,hpp,c,cpp}')
          : arg
      )
    : ['{include,src}/**/*.{h,hpp,c,cpp}']

const files = [...new Set(patterns.flatMap((p) => globSync(p)))].sort()

const problems = []
function report(file, line, message) {
  problems.push(`${file}:${line}: ${message}`)
}

for (const file of files) {
  const lines = fs.readFileSync(file, 'utf8').split('\n')

  lines.forEach((line, i) => {
    if (line.length > maxLineLength) {
      report(file, i + 1, `line too long (${line.length})`)
    }
    if (line.includes('\t')) {
      report(file, i + 1, 'tab character')
    }
  })

  // Doxygen blocks: `/**` on a line of its own, up to `*/`.
  for (let i = 0; i < lines.length; ++i) {
    if (!/^\s*\/\*\*\s*$/.test(lines[i])) continue

    const start = i
    const indent = lines[i].indexOf('/')
    let end = i + 1
    while (end < lines.length && !lines[end].includes('*/')) ++end
    const block = lines.slice(start + 1, end)

    block.forEach((line, k) => {
      const n = start + 2 + k
      if (line.trim() === '') {
        report(file, n, 'empty line inside the comment block')
      } else if (!line.startsWith(' '.repeat(indent + 1) + '*')) {
        report(file, n, 'comment line not aligned with its `/**`')
      }
      const text = prose(line)
      const american = text.match(americanSpellings)
      if (american) report(file, n, `American spelling '${american[0]}'`)
      const wrong = text.match(wrongWords)
      if (wrong) report(file, n, `avoid '${wrong[0]}'`)
      const contraction = text.match(contractions)
      if (contraction) report(file, n, `contraction '${contraction[0]}'`)
    })

    const body = block.join('\n')
    const brief = body.match(/@brief\s+([\s\S]*?)(?:\n\s*\*\s*\n|@details|$)/)
    const details = body.match(/@details\s+([\s\S]*?)(?:\n\s*\*\s*\n|$)/)
    if (brief && details) {
      const b = normalise(brief[1])
      const d = normalise(details[1])
      const prefix = b.slice(0, Math.max(25, b.length / 2))
      if (b.length > 0 && d.startsWith(prefix)) {
        report(file, start + 1, '@details repeats the @brief')
      }
    }

    // Declarations in the public headers need a @brief; the definitions
    // in `inlines/` and `src/` usually carry only the @details.
    const isDeclarationFile =
      /(^|\/)include\//.test(file) && !/(^|\/)inlines\//.test(file)
    const isGrouping = /@(name|defgroup|addtogroup|\{|\})/.test(body)
    if (isDeclarationFile && !isGrouping && !/@brief\b/.test(body)) {
      report(file, start + 1, 'declaration comment without @brief')
    }

    i = end
  }
}

for (const problem of problems) {
  console.log(problem)
}

console.log(`\n${problems.length} problem(s) found in ${files.length} file(s)`)
process.exit(problems.length > 0 ? 1 : 0)

// ----------------------------------------------------------------------------
