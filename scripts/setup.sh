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
# Filename:   setup.sh
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
  echo -e "    $ansi_white_bold hooks $ansi_reset          | configure git hooks"
  echo -e ""
  echo -e "[$ansi_white_bold options $ansi_reset]"
  echo -e "    $ansi_white_bold --silent $ansi_reset       | execute with less verbosity"

  exit 0
}

# invalid argument message
invalid_argument() {
  echo -e "[$ansi_green_bold $script_name $ansi_reset] <$ansi_red_bold error $ansi_reset> invalid argument" \
          "$(if [[ $1 ]]; then echo -e \'$ansi_magenta_bold $1 $ansi_reset\'; fi)"
  echo -e "|> use '$ansi_yellow_bold $script_name help $ansi_reset' to check the available arguments"

  exit 1
}

# configure git hooks
hooks() {
  echo -e "[$ansi_green_bold hooks $ansi_reset] setting up git hooks"

  echo -e "|> [$ansi_white_bold mkdir $ansi_reset] creating the '$ansi_cyan_bold .git/hooks $ansi_reset' directory"
  mkdir -p .git/hooks

  echo -e "|> [$ansi_white_bold copy $ansi_reset] copying the hooks from '$ansi_yellow_bold scripts/hooks $ansi_reset' to '$ansi_cyan_bold .git/hooks $ansi_reset'"
  find "scripts/hooks" -follow -type f -print | while read -r f; do
    case "$f" in
      *.sh)
        if [ -r "$f" ]; then
          hook="${f##*/}"

          output="${hook%.*}"

          echo -e "|--|> '$ansi_magenta_bold $f $ansi_reset' -> '$ansi_cyan_bold .git/hooks/$output $ansi_reset'";

          cp "$f" ".git/hooks/$output"
          chmod 755 ".git/hooks/$output"
        fi
      ;;
      *) echo -e "[$ansi_green_bold hooks $ansi_reset] <$ansi_blue_bold ignored $ansi_reset> ignoring '$ansi_magenta $f $ansi_reset', not a .sh file";;
    esac
  done
}

# argument handler
case $1 in
  help) usage;;
  hooks) hooks;;
  *) invalid_argument $1;;
esac
