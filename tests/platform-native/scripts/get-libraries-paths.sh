#!/usr/bin/env bash

set -o errexit # Exit if command failed.
set -o pipefail # Exit if pipe failed.
set -o nounset # Exit if variable not set.

# Remove the initial space and instead use '\n'.
IFS=$'\n\t'

if [ $# -lt 1 ]
then
  echo "Usage: get-libraries-paths.sh clang++|g++"
  exit 1
fi

$1 "$@" -print-search-dirs | grep 'libraries: =' | sed -e 's|libraries: =||'

# -----------------------------------------------------------------------------
