#!/usr/bin/env bash

# -----------------------------------------------------------------------------
# DO NOT EDIT! Automatically generated from template file:
# build-helper/templates/common/_micro-os-plus/tests/scripts/show-coverage-liquid.sh
#
# This file is part of the µOS++ project (http://micro-os-plus.github.io).
# Copyright (c) 2024-2026 Liviu Ionescu.  All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can be
# obtained from https://opensource.org/licenses/mit.
#
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Safety settings (see https://gist.github.com/ilg-ul/383869cbb01f61a51c4d).

if [[ ! -z ${DEBUG} ]]
then
  set ${DEBUG} # Activate the expand mode if DEBUG is anything but empty.
else
  DEBUG=""
fi

set -o errexit # Exit if command failed.
set -o pipefail # Exit if pipe failed.
set -o nounset # Exit if variable not set.

# Remove the initial space and instead use '\n'.
IFS=$'\n\t'

# -----------------------------------------------------------------------------
# Identify the script location, to reach, for example, the helper scripts.

script_path="$0"
if [[ "${script_path}" != /* ]]
then
  # Make relative path absolute.
  script_path="$(pwd)/$0"
fi

export script_path
export script_name="$(basename "${script_path}")"

export script_folder_path="$(dirname "${script_path}")"
export script_folder_name="$(basename "${script_folder_path}")"

# =============================================================================

if [ $# -lt 2 ]
then
  echo "Usage: show-coverage.sh <binary> <prof-name>"
  exit 1
fi

# set -x

function run_verbose()
{
  local app_path="$1"
  shift

  echo
  echo "[${app_path} $@]"
  "${app_path}" "$@" 2>&1
}

run_verbose xcrun llvm-profdata merge -sparse  "$2".profraw -o "$2".profdata

sources=${3:-""}

run_verbose xcrun llvm-cov show "$1" -instr-profile="$2".profdata \
  --ignore-filename-regex=xpacks --ignore-filename-regex=tests \
  --show-line-counts-or-regions --show-region-summary \
  --show-branches=count --show-branch-summary \
  --show-instantiations --show-instantiation-summary \
  --show-mcdc --show-mcdc-summary --sources ${sources} | grep -v -E \
  -e '^\s*[0-9]+\|\s*[0-9]*\|(#|\s*/\*|\s+\*)' \
  -e '^\s*[0-9]+\|\s+\|\s*$' \
  -e '^\s*[0-9]+\|\s*[0-9]*\|\s*//' \
  -e '^\s*[0-9]+\|\s*\|\s*&& defined\(' \
  -e '^\s*------+\s*$' \
  -e '^\s*\|\s*Branch \([0-9]+:[0-9]+\):\s*\[True:\s*[1-9][0-9]*,\s*False:\s*[1-9][0-9]*\]' \

run_verbose xcrun llvm-cov report "$1" --instr-profile="$2".profdata \
  --ignore-filename-regex=xpacks --ignore-filename-regex=tests \
  --show-branch-summary --show-instantiation-summary \
  --show-mcdc-summary --show-region-summary \
  --sources ${sources} 
  

echo "$(basename "$1") ok"
exit 0

# -----------------------------------------------------------------------------
