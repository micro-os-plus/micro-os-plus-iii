/*
 * DO NOT EDIT! Automatically generated from template file:
 * build-helper/templates/common/_micro-os-plus/tests/scripts/convert-startup-to-vectors-liquid.mjs
 *
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2022-2026 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

/*
 * Generates a vectors-<device>.c file (µOS++ style) from a CMSIS/HAL
 * startup_<device>.s assembly file, by extracting the vector table
 * entries and instantiating them into the `vectors-liquid.c` Liquid
 * template (expected in a `templates` subfolder next to this script).
 *
 * Usage:
 *   node convert-startup-to-vectors.mjs <startup_xxx.s> [vectors-xxx.c]
 *
 * When the output file is omitted, the result is written to stdout
 * instead (diagnostic messages go to stderr, so stdout carries only
 * the result and remains pipeable).
 */

// ----------------------------------------------------------------------------

import fs from 'fs'
import path from 'path'
import { fileURLToPath } from 'url'
import { Liquid } from 'liquidjs'

// ----------------------------------------------------------------------------

const scriptPath = fileURLToPath(import.meta.url)
const scriptFolderPath = path.dirname(scriptPath)
const scriptName = path.basename(scriptPath)

// ----------------------------------------------------------------------------

/**
 * Parses and validates the command line arguments. Paths are kept as
 * given (not resolved to absolute), so that diagnostics and the
 * generated file's header show the original path. The output file
 * is optional; when omitted, `outputFilePath` is `null` and the caller
 * is expected to write the result to stdout instead.
 *
 * @param {string[]} argv - `process.argv`.
 * @returns {\{ inputFilePath: string, outputFilePath: string | null }\}
 */
const parseArgs = (argv) => {
  if (argv.length < 3) {
    console.error(`Usage: ${scriptName} <input.s> [output.c]`)
    process.exit(1)
  }

  const inputFilePath = argv[2]
  const outputFilePath = argv[3] !== undefined ? argv[3] : null

  if (!fs.existsSync(inputFilePath)) {
    console.error(`missing mandatory input file '${inputFilePath}'...`)
    process.exit(1)
  }

  return { inputFilePath, outputFilePath }
}

// ----------------------------------------------------------------------------

/**
 * Parses the vector table that follows the `g_pfnVectors:` label, i.e.
 * the contiguous run of `.word <symbol>` lines up to (but excluding)
 * the trailing `.size g_pfnVectors, ...` directive.
 *
 * The entries are returned in table order, which is the same order
 * used for the `_interrupt_vectors[]` array in the generated C file
 * (stack pointer, `Reset_Handler`, core exception handlers, then the
 * IRQ handlers, with `0` for reserved slots). A trailing `@ ...`,
 * `// ...`, or `/* ... *​/` comment on a `.word` line, if present, is
 * preserved so it can later be reused as the inline comment in the
 * generated C file.
 *
 * @param {string[]} lines - The input assembly file, split into lines.
 * @returns {\{ symbol: string, comment: string | null }[]} The ordered
 *   list of `.word` entries (handler names, or `0` for reserved/unused
 *   slots), each with its trailing comment, if any.
 */
const parseVectorTable = (lines) => {
  const labelRe = /^(?:g_pfnVectors|IRQ_Vectors)\s*:/
  const wordRe =
    /^\.word\s+(\S+)(?:\s+(?:@\s*(.*)|\/\/\s*(.*)|\/\*\s*(.*?)\s*\*\/\s*))?$/

  // A line that is *entirely* a comment (e.g. a section banner like
  // `/* External Interrupts */`), as opposed to a `.word` line with a
  // trailing comment, which `wordRe` already handles on its own.
  const commentOnlyLineRe = /^(?:\/\*.*\*\/|@.*|\/\/.*)$/

  const labelIndex = lines.findIndex((line) => labelRe.test(line.trim()))
  if (labelIndex === -1) {
    console.error(`could not find the 'g_pfnVectors:' label...`)
    process.exit(1)
  }

  const entries = []
  for (let i = labelIndex + 1; i < lines.length; ++i) {
    const trimmedLine = lines[i].trim()
    if (trimmedLine === '' || commentOnlyLineRe.test(trimmedLine)) {
      // Blank lines and standalone comment lines are tolerated
      // anywhere within the table.
      continue
    }

    const match = wordRe.exec(trimmedLine)
    if (match) {
      const comment = match[2] ?? match[3] ?? match[4] ?? null
      entries.push({ symbol: match[1], comment })
    } else {
      // The first non-blank, non-comment line that isn't a `.word
      // <symbol>` directive (typically `.size g_pfnVectors, ...`)
      // ends the table.
      break
    }
  }

  if (entries.length === 0) {
    console.error(`no '.word' entries found after 'g_pfnVectors:'...`)
    process.exit(1)
  }
  // console.error(entries)
  return entries
}

// ----------------------------------------------------------------------------

const liquidEngine = new Liquid({
  strictFilters: true,
  strictVariables: true,
  lenientIf: true,
})

/**
 * Renders the `vectors-liquid.c` template (expected in a `templates`
 * subfolder next to this script) with the given list of IRQ handlers
 * and the input file's path, relative to the current working
 * directory (as `libraryFilePath`, matching the field name used for
 * the same purpose in `xcdl-export.mjs`). The template's header uses
 * this to record which source file it was generated from.
 *
 * The `handlers` array expects objects with a `symbol` field and a
 * `comment` field. `comment` must be `null` (not an empty string)
 * when there is no comment: liquidjs, like Ruby Liquid, treats `""`
 * as truthy, so `{\% if handler.comment %\}` would otherwise still
 * render an empty trailing `// ` on every line.
 *
 * @param {\{ symbol: string, comment: string | null }[]} handlers
 * @param {string} libraryFilePath
 * @param {boolean} isArmArch6m
 * @param {boolean} isArmArch8m
 * @returns {string} The rendered C source.
 */
const renderVectorsTemplate = (
  handlers,
  libraryFilePath,
  isArmArch6m,
  isArmArch8m
) => {
  const templateFilePath = path.resolve(
    scriptFolderPath,
    'templates',
    'vectors-liquid.c'
  )

  if (!fs.existsSync(templateFilePath)) {
    console.error(`missing mandatory template file '${templateFilePath}'...`)
    process.exit(1)
  }

  const templateContent = fs.readFileSync(templateFilePath, 'utf8')
  // console.error(isArmArch6m)
  // console.error(isArmArch8m)
  return liquidEngine.parseAndRenderSync(templateContent, {
    handlers,
    libraryFilePath,
    isArmArch6m,
    isArmArch8m,
  })
}

// ----------------------------------------------------------------------------

const { inputFilePath, outputFilePath } = parseArgs(process.argv)

console.error()
console.error(`Processing '${inputFilePath}'...`)

const inputLines = fs.readFileSync(inputFilePath, 'utf8').split('\n')
const vectorTable = parseVectorTable(inputLines)

console.error(`Found ${vectorTable.length} vector table entries.`)

if (outputFilePath !== null) {
  console.error(`Writing '${outputFilePath}'...`)
} else {
  console.error('No output file given, will print the result to stdout...')
}

// `vectorTable[4]` (the `MemManage_Handler` slot) is Reserved (`0`)
// on ARMv6-M cores (Cortex-M0/M0+), which have no memory-protection
// fault handler, but is populated on ARMv7-M/ARMv8-M cores. A `0`
// there is therefore a reliable signal that this is an ARMv6-M part.
const isArmArch6m = vectorTable.length > 4 && vectorTable[4].symbol === '0'

// `vectorTable[7]` (the `SecureFault_Handler` slot) is used only
// on ARMv8-M cores, but is populated on ARMv7-M cores. A non-`0`
// there is therefore a reliable signal that this is an ARMv8-M part.
const isArmArch8m = vectorTable.length > 7 && vectorTable[7].symbol != '0'

// The first 16 entries of any Cortex-M vector table are fixed by the
// architecture (initial SP, `Reset_Handler`, and the 14 core exception
// vectors, including reserved slots) and are not device-specific IRQ
// handlers, so they are skipped here.
const CORE_VECTOR_COUNT = 16
const irqEntries = vectorTable.slice(CORE_VECTOR_COUNT)

const libraryFilePath = path.relative(process.cwd(), inputFilePath)
const resultText = renderVectorsTemplate(
  irqEntries,
  libraryFilePath,
  isArmArch6m,
  isArmArch8m
)

if (outputFilePath !== null) {
  fs.rmSync(outputFilePath, { force: true })
  fs.writeFileSync(outputFilePath, resultText)
} else {
  process.stdout.write(resultText)
}

console.error(`Done.`)

// ----------------------------------------------------------------------------
