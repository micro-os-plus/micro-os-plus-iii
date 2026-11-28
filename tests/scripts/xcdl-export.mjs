/*
 * DO NOT EDIT! Automatically generated from template file:
 * build-helper/templates/common/_micro-os-plus/tests/scripts/xcdl-export-liquid.mjs
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

import { existsSync, readFileSync } from 'fs'
import path from 'path'
import { pathToFileURL } from 'url'
import { globSync } from 'glob'
import json5 from 'json5'

// ----------------------------------------------------------------------------

// Note: `process.argv[1]` is not resolved through symlinks (unlike
// `fileURLToPath(import.meta.url)`), matching bash `$0` behaviour. See the
// equivalent comment in `generate-website-commons.mjs`.
const scriptPath = path.resolve(process.argv[1])
const scriptName = path.basename(scriptPath)
const scriptFolderPath = path.dirname(scriptPath)

const projectFolderPath = path.dirname(scriptFolderPath)
const npmHelperMaintenanceScriptsPath = path.join(
  projectFolderPath,
  'node_modules',
  '@xpack',
  'npm-packages-helper',
  'maintenance/scripts'
)

const { substitute } = await import(
  pathToFileURL(
    path.join(npmHelperMaintenanceScriptsPath, 'scripts-helper.mjs')
  ).href
)

// ----------------------------------------------------------------------------

const argv = process.argv.slice(2)

if (argv.length < 1) {
  console.log(
    `Usage: ${scriptName} <xcdl-file-or-pattern> [<xcdl-file-or-pattern>...]`
  )
  process.exit(1)
}

// Expand the arguments that are glob patterns, such as
// `sources/**/xcdl-*.jsonc`; quote them in the command, since the Windows
// command processor does not expand patterns, and a POSIX shell expands
// `**` to a single folder level. The plain paths are kept as they are.
const xcdlJsoncPaths = []
for (const arg of argv) {
  if (/[*?[{]/.test(arg)) {
    const matches = globSync(arg, {
      posix: true,
      ignore: ['**/node_modules/**', '**/xpacks/**', '**/build/**'],
    }).sort()
    if (matches.length === 0) {
      console.error(`no files match '${arg}'...`)
      process.exit(1)
    }
    xcdlJsoncPaths.push(...matches)
  } else {
    xcdlJsoncPaths.push(arg)
  }
}

// ----------------------------------------------------------------------------

// The same processing of the nested `cdlComponents` as in the
// `maintenance/scripts/xcdl-export.mjs` script of the project.
const processComponents = (xcdlJson, xcdlJsoncPath) => {
  if (!Array.isArray(xcdlJson.cdlComponents)) {
    console.error(`missing or invalid cdlComponents in '${xcdlJsoncPath}'...`)
    process.exit(1)
  }

  // Linearise the hierarchy: walk recursively through nested
  // cdlComponents, prepending the parent id to each child id
  // (separated by '.'), and collect all components into a flat array.
  const flattenComponents = (components, parentId) => {
    const result = []
    for (const component of components) {
      const qualifiedId = parentId ? `${parentId}.${component.id}` : component.id
      // Separately extract the options, and no longer put it back.
      const { cdlComponents: children, cdlOptions: options, ...rest } = component
      if (Array.isArray(children) && children.length > 0) {
        const childIds = children.map((child) => `${qualifiedId}.${child.id}`)
        const deps = Array.isArray(rest.dependencies)
          ? [...rest.dependencies, ...childIds]
          : childIds
        result.push({ ...rest, id: qualifiedId, dependencies: deps })
        result.push(...flattenComponents(children, qualifiedId))
      } else {
        result.push({ ...rest, id: qualifiedId })
      }
    }
    return result
  }
  xcdlJson.cdlComponents = flattenComponents(xcdlJson.cdlComponents, null)

  // Topologically sort cdlComponents so that components referenced in
  // another component's `dependencies` appear before that component.

  // The topological sort uses Kahn's algorithm:
  // It builds an in-degree map and a successors map from each component's
  // dependencies (filtered to only IDs that exist within cdlComponents).
  // Components with no intra-list dependencies are enqueued first, then
  // successors are unlocked as their prerequisites are emitted.
  // If a cycle is detected (sorted.length !== components.length), the
  // script exits with an error.

  const components = xcdlJson.cdlComponents

  // Set of all component IDs in the list, used to filter dependencies
  // to only those that refer to other components within the same list.
  const known = new Set(components.map((c) => c.id))

  // Map from component ID to the number of intra-list prerequisites
  // that have not yet been emitted (initially 0 for every component).
  const inDegree = new Map(components.map((c) => [c.id, 0]))

  // Map from component ID to the list of component IDs that depend on
  // it, i.e., the components that become unblocked once it is emitted.
  const successors = new Map(components.map((c) => [c.id, []]))

  // Populate inDegree and successors by examining each component's
  // dependencies. Only intra-list dependencies (those present in
  // `known`) are considered; external references are ignored.
  for (const component of components) {
    for (const depId of component.dependencies ?? []) {
      if (known.has(depId)) {
        // depId must appear before component
        successors.get(depId).push(component.id)
        inDegree.set(component.id, inDegree.get(component.id) + 1)
      }
    }
  }

  // Reverse lookup from ID to the full component object, needed when
  // building the sorted output array during the BFS traversal below.
  const idToComponent = new Map(components.map((c) => [c.id, c]))

  // Seed the queue with every component that has no intra-list
  // prerequisites, then collect the final sorted sequence.
  const queue = [...inDegree.entries()]
    .filter(([, deg]) => deg === 0)
    .map(([id]) => id)

  // Breadth-First Search (BFS) pass of Kahn's algorithm.
  // BFS explores the graph level by level using a FIFO queue: all
  // nodes whose prerequisites are already satisfied are processed
  // before unlocking the next wave of dependents. In each iteration,
  // the front node is removed from the queue and appended to `sorted`
  // (emitted). Its in-degree contribution is then subtracted from
  // every successor; any successor whose in-degree reaches zero has
  // had all its prerequisites emitted and is safe to enqueue next.
  const sorted = []
  while (queue.length > 0) {
    const id = queue.shift()
    sorted.push(idToComponent.get(id))
    for (const successorId of successors.get(id)) {
      const newDeg = inDegree.get(successorId) - 1
      inDegree.set(successorId, newDeg)
      if (newDeg === 0) {
        queue.push(successorId)
      }
    }
  }

  if (sorted.length !== components.length) {
    console.error('Circular dependency detected in cdlComponents!')
    process.exit(1)
  }

  xcdlJson.cdlComponents = sorted
}

const substituteFile = (xcdlJsoncPath) => {
  const outputFolderPath = path.dirname(xcdlJsoncPath)

  console.log()
  console.log(`Processing '${xcdlJsoncPath}'...`)

  if (!existsSync(xcdlJsoncPath)) {
    console.log(`missing mandatory ${xcdlJsoncPath}...`)
    process.exit(1)
  }

  // Read in xcdl-package.jsonc and convert to plain JSON.
  const xcdlJson = json5.parse(readFileSync(xcdlJsoncPath, 'utf8'))
  processComponents(xcdlJson, xcdlJsoncPath)

  const context = {
    jsonFilePath: xcdlJsoncPath,
    outputFolderPath,
    xcdl: xcdlJson,
  }

  console.log()
  console.log(`"xcdl_context": ${JSON.stringify(context, undefined, 2)}`)

  console.log()
  console.log('generating files...')
  console.log()

  substitute({
    context,
    fromAbsoluteFolderPath: path.join(scriptFolderPath, 'templates'),
    fromRelativeFilePath: 'CMakeLists-liquid.txt',
    toAbsoluteFilePath: path.resolve(outputFolderPath, 'CMakeLists.txt'),
    toRelativeFilePath: path.join(outputFolderPath, 'CMakeLists.txt'),
    substitutionPrefix: '',
  })

  substitute({
    context,
    fromAbsoluteFolderPath: path.join(scriptFolderPath, 'templates'),
    fromRelativeFilePath: 'meson-liquid.build',
    toAbsoluteFilePath: path.resolve(outputFolderPath, 'meson.build'),
    toRelativeFilePath: path.join(outputFolderPath, 'meson.build'),
    substitutionPrefix: '',
  })
}

for (const xcdlJsoncPath of xcdlJsoncPaths) {
  substituteFile(xcdlJsoncPath)
}

console.log()
console.log(`'${scriptName} ${argv.join(' ')}' done`)

// ----------------------------------------------------------------------------
