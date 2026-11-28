#!/usr/bin/env bash

# -----------------------------------------------------------------------------
# DO NOT EDIT! Automatically generated from template file:
# build-helper/templates/common/_micro-os-plus/tests/scripts/compare-files-liquid.sh
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
  echo "Usage: compare-files.sh <actual> <expected>"
  exit 1
fi

if [ ! -f "$1" ]
then
  echo "Error: file '$1' not found."
  exit 1
fi  

if [ ! -f "$2" ]
then
  echo "Error: file '$2' not found."
  exit 1
fi

# set -x

# Strip the info lines and the line numbers from both files before comparing.
# On mingw-gcc, hex pointers are printed as exectly 16 hex digits, without 0x.
diff -u \
  <(grep -v -e '^# Running:' -e '^Running:' -e '^# Built with ' -e '^Built with ' "$2" | sed -e 's|0[xX][1-9a-fA-F][0-9a-fA-F]*|0xXXX|g' -e 's|[0-9a-fA-F]\{16\}|0xXXX|g' -e 's|, time: [0-9]*\.[0-9]* ms||g' -e 's|  \(line\(Number\)\{0,1\}\): [0-9][0-9]*|  \1: NNN|g' -e 's|:[0-9][0-9]*, |:NNN, |g' -e 's|:[0-9][0-9]*)|:NNN)|g') \
  <(grep -v -e '^# Running:' -e '^Running:' -e '^# Built with ' -e '^Built with ' "$1" | sed -e 's|0[xX][1-9a-fA-F][0-9a-fA-F]*|0xXXX|g' -e 's|[0-9a-fA-F]\{16\}|0xXXX|g' -e 's|, time: [0-9]*\.[0-9]* ms||g' -e 's|  \(line\(Number\)\{0,1\}\): [0-9][0-9]*|  \1: NNN|g' -e 's|:[0-9][0-9]*, |:NNN, |g' -e 's|:[0-9][0-9]*)|:NNN)|g') \

echo "$(basename "$1") ok"
exit 0

# -----------------------------------------------------------------------------
