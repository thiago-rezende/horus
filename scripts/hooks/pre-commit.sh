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
# Filename:   pre-commit.sh
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

failure() {
  echo -e "[$ansi_green_bold hook $ansi_reset] <$ansi_red_bold error $ansi_reset> '$ansi_white_bold pre-commit $ansi_reset' failed on '$ansi_cyan_bold $1 $ansi_reset'"
  echo -e "|> [$ansi_white_bold log $ansi_reset] check '$ansi_yellow_bold $2 $ansi_reset' for more information"
  exit 1
}

pre_commit() {
  echo -e "[$ansi_green_bold hook $ansi_reset] $ansi_white_bold pre-commit $ansi_reset"

  echo -e "|> [$ansi_white_bold meson $ansi_reset] <$ansi_cyan_bold setup $ansi_reset> creating the build directory"
  meson &> /tmp/hook_pre_commit__meson_setup.log setup build

  if [[ $? -ne 0 ]]; then failure "setup" "/tmp/hook_pre_commit__meson_setup.log"; fi

  echo -e "|> [$ansi_white_bold meson $ansi_reset] <$ansi_cyan_bold compile $ansi_reset> compiling the project"
  meson &> /tmp/hook_pre_commit__meson_compile.log compile -C build

  if [[ $? -ne 0 ]]; then failure "compile" "/tmp/hook_pre_commit__meson_compile.log"; fi

  echo -e "|> [$ansi_white_bold meson $ansi_reset] <$ansi_cyan_bold test $ansi_reset> running the tests"
  meson &> /tmp/hook_pre_commit__meson_test.log test -C build

  if [[ $? -ne 0 ]]; then failure "test" "/tmp/hook_pre_commit__meson_test.log"; fi

  echo -e "|> [$ansi_white_bold ninja $ansi_reset] <$ansi_cyan_bold analysis $ansi_reset> running the static analysis"
  ninja &> /tmp/hook_pre_commit__ninja_analysis.log -C build scan-build

  if [[ $? -ne 0 ]]; then failure "analysis" "/tmp/hook_pre_commit__ninja_analysis.log"; fi

  echo -e "|> [$ansi_white_bold ninja $ansi_reset] <$ansi_cyan_bold lint $ansi_reset> running linter"
  ninja &> /tmp/hook_pre_commit__ninja_lint.log -C build clang-format-check

  if [[ $? -ne 0 ]]; then failure "lint" "/tmp/hook_pre_commit__ninja_lint.log"; fi
}

case $1 in
    *) pre_commit;;
esac
