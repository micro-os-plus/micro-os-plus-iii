/*
 * DO NOT EDIT! Automatically generated from template file:
 * build-helper/templates/common/_micro-os-plus/tests/scripts/trigger-workflow-test-all-liquid.mjs
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

// Trigger the `test-all.yml` workflow (manual `workflow_dispatch`) on GitHub,
// using `curl` and the GitHub REST API.
//
// Usage: node trigger-workflow-test-all.mjs [<branch>]
//
// Without arguments, the workflow runs on the current Git branch.
//
// The environment variable GITHUB_API_DISPATCH_TOKEN must hold a token
// with the permission to write Actions (a classic token with the `repo`
// scope, or a fine-grained token with the `Actions: Read and write`
// permission on this repository).

import { spawnSync } from 'child_process'

const workflowFile = 'test-all.yml'

if (process.argv.length > 3) {
  console.error('Error: too many arguments.')
  console.error('Usage: node trigger-workflow-test-all.mjs [<branch>]')
  process.exit(1)
}

// Derive `<owner>/<repository>` from the URL of the `origin` remote.
const originResult = spawnSync('git', ['remote', 'get-url', 'origin'], {
  encoding: 'utf8'
})
const originMatch = originResult.stdout?.trim()
  .match(/github\.com[:/]([^/]+\/[^/]+?)(?:\.git)?$/)
if (originResult.status !== 0 || !originMatch) {
  console.error("Error: cannot determine the GitHub repository from 'origin'.")
  process.exit(1)
}
const repository = originMatch[1]

let branch = process.argv[2]
if (!branch) {
  const result = spawnSync('git', ['rev-parse', '--abbrev-ref', 'HEAD'], {
    encoding: 'utf8'
  })
  if (result.status !== 0) {
    console.error('Error: cannot determine the current branch.')
    process.exit(1)
  }
  branch = result.stdout.trim()
}

const token = process.env.GITHUB_API_DISPATCH_TOKEN
if (!token) {
  console.error('Error: GITHUB_API_DISPATCH_TOKEN is not set.')
  process.exit(1)
}

const apiUrl = `https://api.github.com/repos/${repository}` +
  `/actions/workflows/${workflowFile}/dispatches`

console.log(`Triggering ${workflowFile} on branch '${branch}'...`)

// The headers, which include the token, are passed via stdin (`-H @-`),
// to keep the token out of the process list.
const headers = [
  `Authorization: Bearer ${token}`,
  'Accept: application/vnd.github+json',
  'X-GitHub-Api-Version: 2022-11-28',
  'Content-Type: application/json'
].join('\n')

const result = spawnSync('curl', [
  '--silent', '--show-error',
  '--output', '-',
  '--write-out', '\n%{http_code}\n',
  '--request', 'POST',
  '--header', '@-',
  '--data', JSON.stringify({ ref: branch }),
  apiUrl
], { input: headers, encoding: 'utf8' })

if (result.error) {
  console.error(`Error: cannot run 'curl' (${result.error.message}).`)
  process.exit(1)
}

if (result.status !== 0) {
  console.error(result.stderr)
  process.exit(result.status ?? 1)
}

const lines = result.stdout.trimEnd().split('\n')
const httpCode = lines.pop()
const body = lines.join('\n').trim()

// The API replies with 204 (no content) on success.
if (httpCode !== '204') {
  console.error(`Error: GitHub replied with HTTP ${httpCode}.`)
  if (body) {
    console.error(body)
  }
  process.exit(1)
}

console.log('Done. See https://github.com/' + repository +
  '/actions/workflows/' + workflowFile)

// ----------------------------------------------------------------------------
