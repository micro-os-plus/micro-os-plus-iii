/*
 * DO NOT EDIT! Automatically generated from template file:
 * npm-packages-helper/templates/common/.github/skills/_micro-os-plus/doxygen-review/scripts/check-doxygen-merged-liquid.mjs
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

// Report the documented entities whose merged Doxygen documentation lacks
// a brief or a details paragraph, by parsing the Doxygen XML output.
//
// Doxygen merges the comment of a declaration (usually with the @brief) and
// the comment of the definition (usually with the @details, in the
// `inlines/` or `src/` files), so the completeness can be checked reliably
// only on the merged result, not on the individual comment blocks.
//
// Usage:
//   node check-doxygen-merged.mjs <xml-folder>
//
// Only entities located in the `include/` and `src/` folders are checked.
// The exit code is 1 if any problem is found.

import fs from 'fs'
import path from 'path'

// ----------------------------------------------------------------------------

const xmlFolderPath = process.argv[2]
if (!xmlFolderPath || !fs.existsSync(xmlFolderPath)) {
  console.error('Usage: node check-doxygen-merged.mjs <xml-folder>')
  process.exit(2)
}

// The `simplesect` kinds that do not count as a details paragraph.
const nonDetailsSimpleSects = [
  'return',
  'see',
  'since',
  'par',
  'pre',
  'post',
  'author',
  'version',
  'date',
  'copyright',
]

function stripTags(xml) {
  return xml
    .replace(/<[^>]+>/g, ' ')
    .replace(/&[a-z]+;/g, ' ')
    .trim()
}

function firstElement(xml, tag) {
  const match = xml.match(new RegExp(`<${tag}>([\\s\\S]*?)</${tag}>`))
  return match ? match[1] : ''
}

function hasBrief(xml) {
  return stripTags(firstElement(xml, 'briefdescription')) !== ''
}

function hasDetails(xml) {
  let detailed = firstElement(xml, 'detaileddescription')
  detailed = detailed
    .replace(/<parameterlist[\s\S]*?<\/parameterlist>/g, '')
    .replace(/<xrefsect[\s\S]*?<\/xrefsect>/g, '')
    .replace(/<programlisting[\s\S]*?<\/programlisting>/g, '')
  for (const kind of nonDetailsSimpleSects) {
    detailed = detailed.replace(
      new RegExp(`<simplesect kind="${kind}">[\\s\\S]*?</simplesect>`, 'g'),
      ''
    )
  }
  return stripTags(detailed) !== ''
}

function location(xml) {
  const match = xml.match(/<location file="([^"]*)"(?: line="(\d+)")?/)
  return match ? { file: match[1], line: match[2] ?? '?' } : null
}

function decodeEntities(text) {
  return text
    .replace(/&lt;/g, '<')
    .replace(/&gt;/g, '>')
    .replace(/&quot;/g, '"')
    .replace(/&apos;/g, "'")
    .replace(/&amp;/g, '&')
}

function isInScope(filePath) {
  return /(^|\/)(include|src)\//.test(filePath)
}

function relative(filePath) {
  return filePath.replace(/^.*?\/((include|src)\/)/, '$1')
}

// ----------------------------------------------------------------------------

const problems = []
const seen = new Set()

const xmlFileNames = fs
  .readdirSync(xmlFolderPath)
  .filter((name) => name.endsWith('.xml') && name !== 'index.xml')

for (const xmlFileName of xmlFileNames) {
  const text = fs.readFileSync(path.join(xmlFolderPath, xmlFileName), 'utf8')

  for (const compound of text.matchAll(
    /<compounddef\b([^>]*)>([\s\S]*?)<\/compounddef>/g
  )) {
    const kind = (compound[1].match(/kind="([^"]*)"/) ?? [])[1]
    const body = compound[2]
    const name = firstElement(body, 'compoundname')

    // The compound's own description, without the members and the code.
    const own = body
      .replace(/<sectiondef[\s\S]*?<\/sectiondef>/g, '')
      .replace(/<programlisting[\s\S]*?<\/programlisting>/g, '')
      .replace(/<templateparamlist[\s\S]*?<\/templateparamlist>/g, '')
    const ownLocation = location(own)

    if (
      ['class', 'struct', 'union', 'file'].includes(kind) &&
      ownLocation &&
      isInScope(ownLocation.file) &&
      !seen.has(`${kind}:${name}`)
    ) {
      seen.add(`${kind}:${name}`)
      const missing = []
      if (!hasBrief(own)) missing.push('brief')
      if (!hasDetails(own)) missing.push('details')
      if (missing.length > 0) {
        problems.push({
          ...ownLocation,
          kind,
          name: decodeEntities(name),
          missing,
        })
      }
    }

    for (const member of body.matchAll(
      /<memberdef\b([^>]*)>([\s\S]*?)<\/memberdef>/g
    )) {
      const id = (member[1].match(/id="([^"]*)"/) ?? [])[1]
      if (seen.has(id)) continue
      seen.add(id)

      const memberKind = (member[1].match(/kind="([^"]*)"/) ?? [])[1]
      // Remove the nested elements that have their own descriptions.
      const memberBody = member[2]
        .replace(/<enumvalue[\s\S]*?<\/enumvalue>/g, '')
        .replace(/<templateparamlist[\s\S]*?<\/templateparamlist>/g, '')
        .replace(/<param>[\s\S]*?<\/param>/g, '')
      const memberLocation = location(memberBody)
      if (!memberLocation || !isInScope(memberLocation.file)) continue

      const missing = []
      if (!hasBrief(memberBody)) missing.push('brief')
      if (!hasDetails(memberBody)) missing.push('details')
      if (missing.length > 0) {
        problems.push({
          ...memberLocation,
          kind: memberKind,
          name: decodeEntities(
            `${name}::${firstElement(memberBody, 'name')}`
          ),
          missing,
        })
      }
    }
  }
}

problems.sort((a, b) =>
  a.file === b.file
    ? Number(a.line) - Number(b.line)
    : a.file.localeCompare(b.file)
)

for (const p of problems) {
  console.log(
    `${relative(p.file)}:${p.line}: ${p.kind} ${p.name}: ` +
      `missing ${p.missing.join(' and ')}`
  )
}

console.log(`\n${problems.length} problem(s) found`)
process.exit(problems.length > 0 ? 1 : 0)

// ----------------------------------------------------------------------------
