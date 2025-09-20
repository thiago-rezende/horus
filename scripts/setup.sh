#!/bin/bash

# ╔═╗╔═╗╔╦╗╦ ╦╔═╗
# ╚═╗║╣  ║ ║ ║╠═╝
# ╚═╝╚═╝ ╩ ╚═╝╩
#
# Filename:   setup.sh
# GitHub:     https://github.com/thiago-rezende
# Maintainer: Thiago Rezende <thiago.manoel.rezende@gmail.com>

# script variables
script_name=`basename "$0"`
script_version='v1.0.0'
script_description='general purpose setup script'

# script directories
script_directory=`dirname "$0"`
script_logs_directory=${SCRIPT_LOGS_DIRECTORY:-"${script_directory}/logs"}

# script verbosity variables
script_verbose_output=${SCRIPT_VERBOSE_OUTPUT:-1}

# positional arguments
positional_arguments=()

# ansi colors
declare -r                               \
        ansi_red='\033[31m'              \
        ansi_blue='\033[34m'             \
        ansi_cyan='\033[36m'             \
        ansi_gray='\033[30m'             \
        ansi_white='\033[37m'            \
        ansi_green='\033[32m'            \
        ansi_yellow='\033[33m'           \
        ansi_magenta='\033[35m'          \
        ansi_red_bold='\033[0;31;1m'     \
        ansi_blue_bold='\033[0;34;1m'    \
        ansi_cyan_bold='\033[0;36;1m'    \
        ansi_black_bold='\033[0;30;1m'   \
        ansi_green_bold='\033[0;32;1m'   \
        ansi_white_bold='\033[0;37;1m'   \
        ansi_yellow_bold='\033[0;33;1m'  \
        ansi_magenta_bold='\033[0;35;1m' \
        ansi_reset='\033[0m'

# script usage message
script__usage() {
  echo -e "[$ansi_green_bold $script_name $ansi_reset]$ansi_white <$ansi_yellow_bold ${script_version}$ansi_reset$ansi_white > $ansi_reset"
  echo -e "  $ansi_blue_bold $script_description $ansi_reset"
  echo -e ""
  echo -e "[$ansi_white_bold usage $ansi_reset]"
  echo -e "  $ansi_green $script_name $ansi_white utility $ansi_yellow command $ansi_magenta < argument > $ansi_cyan [ options ] $ansi_reset"
  echo -e ""
  echo -e "[$ansi_white_bold utilities $ansi_reset]"
  echo -e "  $ansi_white help $ansi_reset   - show this help message"
  echo -e "  $ansi_white hooks $ansi_reset  - execute the git$ansi_white hooks$ansi_reset utility"
  echo -e ""
  echo -e "[$ansi_white_bold hooks $ansi_reset]"
  echo -e "  $ansi_yellow apply $ansi_reset  - apply the available git hooks to the repository"
  echo -e ""
  echo -e "[$ansi_white_bold options $ansi_reset]"
  echo -e "   $ansi_cyan --quiet $ansi_reset  - reduce verbosity"
  echo -e ""

  exit 0
}

# generic validation error
invalid() {
  local kind=$1
  local object=$2
  local ansi_color=${3:-"${ansi_yellow}"}

  if [ -z "$object" ]; then
    script__usage
  else
    echo >&2 -e "[$ansi_red error $ansi_reset] invalid $kind '$ansi_color $object $ansi_reset'"
    echo >&2 -e "|- run '$ansi_green ${script_name}$ansi_white help $ansi_reset' to check the script usage"
  fi

  exit 1
}

# generic missing error
missing() {
  local kind=$1
  local ansi_color=${3:-"${ansi_yellow}"}

  echo >&2 -e "[$ansi_red error $ansi_reset] missing '$ansi_color $kind $ansi_reset'"
  echo >&2 -e "|- run '$ansi_green ${script_name}$ansi_white help $ansi_reset' to check the script usage"

  exit 1
}

# generic failure procedure
failure() {
  local utility=$1
  local command=$2

  local output=$3

  echo >&2 -e "[$ansi_red error $ansi_reset] utility '$ansi_white $utility $ansi_reset' failed on '$ansi_yellow $command $ansi_reset'"
  echo >&2 -e "|- [$ansi_white log $ansi_reset] check '$ansi_yellow $output $ansi_reset' for more information"

  exit 1
}

# setup verbosity
setup__verbosity() {
  if [ "${script_verbose_output}" -ne 0 ]; then
      exec 3>&1
  else
      exec 3>/dev/null
  fi
}

# setup logging
setup__logging() {
  if [ -d $script_logs_directory ]; then
    return
  fi

  echo >&3 -e "[$ansi_white logging $ansi_reset] settnig up the '$ansi_cyan logging $ansi_reset' environment"
  echo >&3 -e "|- [$ansi_white mkdir $ansi_reset] creating the '$ansi_yellow ${script_logs_directory}$ansi_reset ' directory"

  mkdir >&/tmp/${script_name%.*}__setup__logging__mkdir.log -p $script_logs_directory

  if [ $? -ne 0 ]; then
    failure "setup" "logging" "/tmp/${script_name%.*}__setup__logging__mkdir.log"
  fi
}

# hooks handler
hooks__handler() {
  setup__logging

  local command=$1

  case $command in
    apply) hooks__apply "${@:2}";;
    *) if [ -z $command ]; then missing "command" $ansi_yellow; else invalid "command" $command $ansi_yellow; fi;;
  esac
}

# hooks apply procedure
hooks__apply() {
  echo >&3 -e "[$ansi_white hooks $ansi_reset] applying the available '$ansi_blue hooks $ansi_reset'"

  echo -e "|- [$ansi_white mkdir $ansi_reset] creating the '$ansi_blue .git/hooks $ansi_reset' directory"

  mkdir >&${script_logs_directory}/setup__hooks__apply__mkdir.log -p .git/hooks

  if [ $? -ne 0 ]; then
    failure "hooks" "apply" "${script_logs_directory}/setup__hooks__apply__mkdir.log"
  fi

  find "scripts/hooks" -follow -type f -print | while read -r f; do
    case "$f" in
      *.sh)
        if [ -r "$f" ]; then
          hook="${f##*/}"

          output="${hook%.*}"

          echo -e "|- [$ansi_white cp $ansi_reset] copying '$ansi_blue $f $ansi_reset' to '$ansi_blue .git/hooks/$output $ansi_reset'";

          cp "$f" ".git/hooks/$output"
          chmod 755 ".git/hooks/$output"
        fi
      ;;
      *) echo -e "|- [$ansi_green_bold skip $ansi_reset] ignoring '$ansi_magenta $f $ansi_reset' since it is not a .sh file";;
    esac
  done
}

# options handler
while [ $# -gt 0 ]; do
  case $1 in
    # quiet option
    --quiet) script_verbose_output=0; shift;;
    # usage option
    -h|--help) script__usage;;
    # invalid option
    -*|--*) invalid "option" $1 $ansi_cyan;;
    # positional argument
    *) positional_arguments+=("$1"); shift;;
  esac
done

set -- "${positional_arguments[@]}"

# setup script verbosity
setup__verbosity

# argument handler
case $1 in
  help) script__usage;;
  hooks) hooks__handler "${@:2}";;
  *) invalid "utility" ${1:-""} $ansi_white;;
esac
