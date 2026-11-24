/*
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

// Build a probe program against the objects of an existing test build, and
// run it, natively or on QEMU.
//
// The µOS++ IIIe sources cannot be built alone: they need a port, the
// application configuration, and the platform files. The probe therefore
// reuses a configured and built test folder (for example
// `tests/build/native-cmake-clang-debug`): it is compiled with the flags of
// the `unit` test (`tests/sources/unit/src/test-rtos.cpp`, which includes the
// `unit` `os-app-config.h`), and linked with all the objects of `unit-test`,
// except those of the `unit` test itself. The probe defines `os_main()`.
//
// The warnings are reported but are not errors (`-Wno-error`), since the
// test builds use `-Weverything -Werror` with clang.
//
// Usage:
//   node build-probe.mjs <tests-build-folder> <probe.cpp> [--no-run]
//     [-- <extra compiler flags for the probe>]
//
// The executable is written next to the probe source, without the extension
// (natively) or with `.elf` (on QEMU). The exit status is that of the probe.

// ----------------------------------------------------------------------------

import { existsSync, readFileSync, readdirSync } from 'node:fs'
import path from 'node:path'
import { spawnSync } from 'node:child_process'

// ----------------------------------------------------------------------------

const usage = () => {
  console.error(
    'Usage: node build-probe.mjs <tests-build-folder> <probe.cpp> ' +
      '[--no-run] [-- <flags>]'
  )
  process.exit(1)
}

const allArgs = process.argv.slice(2)
const separatorIndex = allArgs.indexOf('--')
const args = separatorIndex >= 0 ? allArgs.slice(0, separatorIndex) : allArgs
// The flags after `--` are added when the probe is compiled (not to the
// objects of the test build, which are reused as they are).
const extraFlags =
  separatorIndex >= 0 ? allArgs.slice(separatorIndex + 1).join(' ') : ''
const noRun = args.includes('--no-run')
const positional = args.filter((arg) => !arg.startsWith('--'))
if (positional.length !== 2) {
  console.error('Error: missing arguments.')
  usage()
}

const buildFolderPath = path.resolve(positional[0])
const probeFilePath = path.resolve(positional[1])

for (const filePath of [
  path.join(buildFolderPath, 'compile_commands.json'),
  path.join(buildFolderPath, 'build.ninja'),
  probeFilePath,
]) {
  if (!existsSync(filePath)) {
    console.error(`Error: '${filePath}' not found; build the tests first.`)
    usage()
  }
}

// ----------------------------------------------------------------------------

const run = (command) => {
  console.log(command.length > 300 ? `${command.slice(0, 300)} ...` : command)
  const result = spawnSync(command, {
    cwd: buildFolderPath,
    shell: '/bin/sh',
    stdio: 'inherit',
  })
  if (result.error) {
    console.error(`Error: ${result.error.message}`)
    process.exit(1)
  }
  if (result.status !== 0) {
    process.exit(result.status ?? 1)
  }
}

// The compile command of a unit test source, without its output and input.
const compileCommands = JSON.parse(
  readFileSync(path.join(buildFolderPath, 'compile_commands.json'), 'utf8')
)
const unitEntry = compileCommands.find((entry) =>
  entry.file.endsWith('sources/unit/src/test-rtos.cpp')
)
if (!unitEntry) {
  console.error('Error: the build folder has no unit test.')
  process.exit(1)
}
const compileCommand = unitEntry.command.replace(/ -o \S+ -c \S+$/, '')
const compiler = compileCommand.split(' ')[0]

// The link flags of `unit-test`, from `build.ninja`.
const ninja = readFileSync(path.join(buildFolderPath, 'build.ninja'), 'utf8')
const linkBlock = ninja.match(
  /^build platform-bin\/unit-test(\.elf)?:[^\n]*\n(?: {2}\w+ = [^\n]*\n)*/m
)
if (!linkBlock) {
  console.error('Error: no link step for unit-test in build.ninja.')
  process.exit(1)
}
const isElf = linkBlock[1] === '.elf'
const linkVariable = (name) => {
  const match = linkBlock[0].match(new RegExp(`^ {2}${name} = (.*)$`, 'm'))
  return match ? match[1] : ''
}
const linkFlags = [
  linkVariable('FLAGS'),
  linkVariable('LINK_FLAGS')
    .replace(/ -v( |$)/, ' ')
    .replace(/-Wl,-Map,\S+/, ''),
].join(' ')
const linkLibraries = [
  linkVariable('LINK_PATH'),
  linkVariable('LINK_LIBRARIES'),
].join(' ')

// The objects of `unit-test`, except those of the unit test sources.
const objectsFolderPath = 'platform-bin/CMakeFiles/unit-test.dir'
const objectFilePaths = readdirSync(
  path.join(buildFolderPath, objectsFolderPath),
  { recursive: true }
)
  // `.o` natively, `.obj` with the cross toolchains.
  .filter((name) => /\.o(bj)?$/.test(name))
  .filter((name) => !name.includes(path.join('sources', 'unit')))
  .map((name) => path.join(objectsFolderPath, name))

const outputFilePath =
  probeFilePath.replace(/\.(cpp|cc|c)$/, '') + (isElf ? '.elf' : '')

run(
  `${compileCommand} -Wno-error -Wno-c++98-compat ${extraFlags} ` +
    `-c '${probeFilePath}' -o '${outputFilePath}.o'`
)
run(
  `${compiler} ${linkFlags} '${outputFilePath}.o' ` +
    `${objectFilePaths.join(' ')} -o '${outputFilePath}' ${linkLibraries}`
)

if (noRun) {
  process.exit(0)
}

if (!isElf) {
  run(`'${outputFilePath}'`)
  process.exit(0)
}

// On QEMU, run it as CTest runs `unit-test`.
const ctestFile = readdirSync(buildFolderPath, { recursive: true })
  .filter((name) => name.endsWith('CTestTestfile.cmake'))
  .map((name) => readFileSync(path.join(buildFolderPath, name), 'utf8'))
  .join('\n')
const testMatch = ctestFile.match(/add_test\(\[=\[unit-test\]=\] (.*)\)/)
if (!testMatch) {
  console.error('Error: no unit-test in the CTest files.')
  process.exit(1)
}
const qemuArgs = [...testMatch[1].matchAll(/"([^"]*)"/g)].map((match) =>
  match[1]
    .replace('unit-test.elf', outputFilePath)
    .replace('arg=unit-test', 'arg=probe')
)
run(
  `PATH="${path.join(buildFolderPath, 'xpacks', '.bin')}:$PATH" ` +
    qemuArgs.map((arg) => `'${arg}'`).join(' ')
)

// ----------------------------------------------------------------------------
