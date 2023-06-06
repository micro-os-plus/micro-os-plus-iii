# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ distribution.
#   (https://github.com/micro-os-plus/)
# Copyright (c) 2020 Liviu Ionescu
#
# Permission to use, copy, modify, and/or distribute this software
# for any purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can
# be obtained from https://opensource.org/licenses/MIT/.
#
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Common functions used in various tests scripts.
# -----------------------------------------------------------------------------

function run_verbose()
{
  # Does not include the .exe extension.
  local app_path=$1
  shift

  echo
  echo "[${app_path} $@]"
  "${app_path}" "$@" 2>&1
}

function trigger_github_workflow()
{
  local github_org="$1"
  local github_repo="$2"
  local workflow_id="$3"
  local data_file_path="$4"
  local token="$5"

  echo
  echo "Request body:"
  cat "${data_file_path}"

  # This script requires an authentication token in the environment.
  # https://docs.github.com/en/rest/reference/actions#create-a-workflow-dispatch-event

  echo
  echo "Response:"

  curl \
    --request POST \
    --include \
    --header "Authorization: token ${token}" \
    --header "Content-Type: application/json" \
    --header "Accept: application/vnd.github.v3+json" \
    --data-binary @"${data_file_path}" \
    https://api.github.com/repos/${github_org}/${github_repo}/actions/workflows/${workflow_id}/dispatches

  rm -rf "${data_file_path}"
}

function exit_trap()
{
  if [ -f "/.dockerenv" ]
  then
    # For self-hosted runners, remove the root owned folders.
    run_verbose xpm run deep-clean
  fi

  if [ -n "$(which lsb_release)" ]
  then
    run_verbose lsb_release -a
  fi
}

# -----------------------------------------------------------------------------
