#! /usr/bin/env bash

#
# ╔═╗╦ ╦╔═╗╔╗╔╔═╗╔═╗╦  ╔═╗╔═╗
# ║  ╠═╣╠═╣║║║║ ╦║╣ ║  ║ ║║ ╦
# ╚═╝╩ ╩╩ ╩╝╚╝╚═╝╚═╝╩═╝╚═╝╚═╝
#
# Filename:   changelog.sh
# GitHub:     https://github.com/thiago-rezende
# Maintainer: Thiago Rezende <thiago.manoel.rezende@gmail.com>

# script variables
script_name=`basename "$0"`
script_version='v1.0.0'
script_description='general purpose changelog script'

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
  echo -e "  $ansi_white help $ansi_reset     - show this '$ansi_white help $ansi_reset' message"
  echo -e "  $ansi_white generate $ansi_reset - execute the '$ansi_white generate $ansi_reset' utility"
  echo -e ""
  echo -e "[$ansi_white_bold generate $ansi_reset]"
  echo -e "  $ansi_yellow plain  $ansi_magenta < file > $ansi_white_bold - generate a plain changelog"
  echo -e "  $ansi_yellow tagged $ansi_magenta < file > $ansi_white_bold - generate a tagged changelog"
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

# generate handler
generate__handler() {
  setup__logging

  local command=$1

  case $command in
    plain) generate__plain "${@:2}";;
    tagged) generate__tagged "${@:2}";;
    *) if [ -z $command ]; then missing "command" $ansi_yellow; else invalid "command" $command $ansi_yellow; fi;;
  esac
}

# generate plain procedure
generate__plain() {
  if [ -z $1 ]; then
    missing "file"
  fi

  local file=$1

  echo >&3 -e "[$ansi_white generate $ansi_reset] generating a '$ansi_blue plain $ansi_reset' changelog in '$ansi_blue $file $ansi_reset'"

  header="# CHANGELOG
> This changelog is automaticaly generated.
> If you find any issues with this file, please report to the developers.

## [Unreleased]
"

  echo >&3 -e "|- [$ansi_white echo $ansi_reset] writing the '$ansi_blue header $ansi_reset'"

  echo "$header" > $file

  echo >&3 -e "|- [$ansi_white echo $ansi_reset] writing the '$ansi_blue commits $ansi_reset'"

  git log "HEAD" --pretty=" - [%h] %s (%an)" >> $file
}

# generate tagged procedure
generate__tagged() {
  if [ -z $1 ]; then
    missing "file"
  fi

  local file=$1
  local tags=$(git tag --sort=-v:refname)

  if [ -z ${tags[*]} ]; then
    echo "repository doesn't have any git tags" > ${script_logs_directory}/changelog__generate__tagged__validation.log

    failure "generate" "tagged" "${script_logs_directory}/changelog__generate__tagged__validation.log"
  fi

  echo >&3 -e "[$ansi_white generate $ansi_reset] generating a '$ansi_blue tagged $ansi_reset' changelog in '$ansi_blue $file $ansi_reset'"

  header="# CHANGELOG
> This changelog is automaticaly generated.
> If you find any issues with this file, please report to the developers.

## [Unreleased]
"

  echo >&3 -e "|- [$ansi_white echo $ansi_reset] writing the '$ansi_blue header $ansi_reset'"

  echo "$header" > $file

  function __log_between_tags() {
    local start_tag=$1
    local end_tag=$2

    if [ "$end_tag" == "HEAD" ]; then
      git log $start_tag..$end_tag --pretty=" - [%h] %s (%an)" >> $file
    elif [ -z $end_tag ]; then
      echo "## [$start_tag]" >> $file

      git log $start_tag --pretty=" - [%h] %s (%an)" >> $file
    else
      echo "## [$end_tag]" >> $file

      git log $start_tag..$end_tag --pretty=" - [%h] %s (%an)" >> $file
    fi

    echo "" >> $file
  }

  current=("HEAD")
  previous=""

  for p in ${tags[@]}; do
    previous=$p

    echo >&3 -e "|- [$ansi_white echo $ansi_reset] writing the '$ansi_blue commits $ansi_reset' between '$ansi_blue $previous..$current $ansi_reset'"

    __log_between_tags $previous $current

    current=$previous
  done

  __log_between_tags $previous
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
  generate) generate__handler "${@:2}";;
  *) invalid "utility" ${1:-""} $ansi_white;;
esac
