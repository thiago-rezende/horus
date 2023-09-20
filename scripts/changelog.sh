#!/bin/bash

#   ██░ ██  ▒█████   ██▀███   █    ██   ██████
#  ▓██░ ██▒▒██▒  ██▒▓██ ▒ ██▒ ██  ▓██▒▒██    ▒
#  ▒██▀▀██░▒██░  ██▒▓██ ░▄█ ▒▓██  ▒██░░ ▓██▄
#  ░▓█ ░██ ▒██   ██░▒██▀▀█▄  ▓▓█  ░██░  ▒   ██▒
#  ░▓█▒░██▓░ ████▓▒░░██▓ ▒██▒▒▒█████▓ ▒██████▒▒
#   ▒ ░░▒░▒░ ▒░▒░▒░ ░ ▒▓ ░▒▓░░▒▓▒ ▒ ▒ ▒ ▒▓▒ ▒ ░
#   ▒ ░▒░ ░  ░ ▒ ▒░   ░▒ ░ ▒░░░▒░ ░ ░ ░ ░▒  ░ ░
#   ░  ░░ ░░ ░ ░ ▒    ░░   ░  ░░░ ░ ░ ░  ░  ░
#   ░  ░  ░    ░ ░     ░        ░           ░
#
# Filename:   changelog.sh
# GitHub:     https://github.com/thiago-rezende
# Maintainer: Thiago Rezende <thiago.manoel.rezende@gmail.com>

# ANSI colors
declare -r                               \
        ansi_black='\033[30m'            \
        ansi_black_bold='\033[0;30;1m'   \
        ansi_red='\033[31m'              \
        ansi_red_bold='\033[0;31;1m'     \
        ansi_green='\033[32m'            \
        ansi_green_bold='\033[0;32;1m'   \
        ansi_yellow='\033[33m'           \
        ansi_yellow_bold='\033[0;33;1m'  \
        ansi_blue='\033[34m'             \
        ansi_blue_bold='\033[0;34;1m'    \
        ansi_magenta='\033[35m'          \
        ansi_magenta_bold='\033[0;35;1m' \
        ansi_cyan='\033[36m'             \
        ansi_cyan_bold='\033[0;36;1m'    \
        ansi_white='\033[37m'            \
        ansi_white_bold='\033[0;37;1m'   \
        ansi_reset='\033[0m'
declare -r ansi_grey="$ansi_black_bold"

# script file name
script_name=`basename "$0"`

# usage message
usage() {
  echo -e "[$ansi_green_bold $script_name $ansi_reset] usage:"
  echo -e "   $ansi_yellow_bold $script_name $ansi_white_bold command $ansi_cyan_bold [ options ] $ansi_reset"
  echo -e ""
  echo -e "[$ansi_white_bold commands $ansi_reset]"
  echo -e "    $ansi_white_bold help $ansi_reset           | show this help message"
  echo -e "    $ansi_white_bold plain $ansi_reset          | generate a plain changelog"
  echo -e "    $ansi_white_bold markdown $ansi_reset       | generate a markdown changelog"
  echo -e ""
  echo -e "[$ansi_white_bold options $ansi_reset]"
  echo -e "    $ansi_white_bold --tags $ansi_reset         | use tags"
  echo -e "    $ansi_white_bold --conventional $ansi_reset | use conventional commits"

  exit 0
}

# invalid argument message
invalid_argument() {
  echo -e "[$ansi_green_bold $script_name $ansi_reset] <$ansi_red_bold error $ansi_reset> invalid argument" \
          "$(if [[ $1 ]]; then echo -e \'$ansi_magenta_bold $1 $ansi_reset\'; fi)"
  echo -e "|> use '$ansi_yellow_bold $script_name help $ansi_reset' to check the available arguments"

  exit 1
}

# git utilities
function __log_between_tags() {
  if [[ $2 == "HEAD" ]]; then
    git log $1..$2 --pretty=" - [%h] %s (%an)" >> $file
  elif [[ -z $2 ]]; then
    echo "## [$1]" >> $file
    git log $1 --pretty=" - [%h] %s (%an)" >> $file
  else
    echo "## [$2]" >> $file
    git log $1..$2 --pretty=" - [%h] %s (%an)" >> $file
  fi
  echo "" >> $file
}

# markdown generator
markdown() {
  local file=CHANGELOG.md
  local tags=$(git tag --sort=-v:refname)

  header="# CHANGELOG
> This changelog is automaticaly generated.
> If you find any issues with this file, please report to the developers.

## [Unreleased]
"

  echo "$header" > $file

  if [[ -z ${tags[*]} ]]; then
    echo -e "[$ansi_yellow_bold warn $ansi_reset] repository doesn't have any$ansi_cyan_bold git tags$ansi_reset"
    echo -e "|> [$ansi_white_bold generating $ansi_reset] plain changelog"

    git log "HEAD" --pretty=" - [%h] %s (%an)" >> $file

    exit 0
  else
    echo -e "[$ansi_green_bold info $ansi_reset] repository have$ansi_cyan_bold git tags$ansi_reset"
    echo -e "|> [$ansi_white_bold generating $ansi_reset] tagged changelog"

    tag=("HEAD")
    prev=""

    for p in ${tags[@]}; do
      prev=$p

      __log_between_tags $prev $tag

      tag=$prev
    done

    __log_between_tags $prev

    exit 0
  fi
}

# argument handler
case $1 in
  help) usage;;
  markdown) markdown;;
  *) invalid_argument $1;;
esac
