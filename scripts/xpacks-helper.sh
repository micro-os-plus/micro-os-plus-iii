#!/bin/bash
#set -euo pipefail
#IFS=$'\n\t'

# -----------------------------------------------------------------------------
# Bash helper script used in project generate.sh scripts.
# -----------------------------------------------------------------------------

# Optional args: src folders, like posix-io, driver
do_add_micro_os_plus_iii_xpack() {
  local pack_name='micro-os-plus-iii'
  do_tell_xpack "${pack_name}-xpack"

  do_select_pack_folder "ilg/${pack_name}.git"

  # Exception to the rule, folder is micro-os-plus, not cmsis-plus; 
  # The package will be renamed.
  do_prepare_dest "${pack_name}/include"
  do_add_content "${pack_folder}/include"/* 

  do_prepare_dest "${pack_name}/src"
  do_add_content "${pack_folder}/src/diag" 
  do_add_content "${pack_folder}/src/libc" 
  do_add_content "${pack_folder}/src/libcpp" 
  do_add_content "${pack_folder}/src/rtos" 
  do_add_content "${pack_folder}/src/semihosting" 
  do_add_content "${pack_folder}/src/startup" 
  do_add_content "${pack_folder}/src/memory" 
  do_add_content "${pack_folder}/src/utils" 

  while [ $# -ge 1 ]
  do
    do_add_content "${pack_folder}/src/$1" 
    shift
  done
}

# -----------------------------------------------------------------------------
