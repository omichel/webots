#!/bin/bash

# Usage: msys64_installer.sh [--all]
# Options:
# --all: install all the optional dependencies

declare -a BASE_PACKAGES=(
  "make"                      # Makefile
  "mingw-w64-x86_64-gcc"      # C/C++ compiler
  "tar"                       # Webots dependencies
  "unzip"                     # Webots dependencies
  "zip"                       # robotbenchmark square path
  "mingw-w64-x86_64-qt5"      # Webots
  "mingw-w64-x86_64-qtwebkit" # Webots
  "mingw-w64-x86_64-libzip"   # Webots
  "mingw-w64-x86_64-libgd"    # Webots
  "liblzma"                   # Webots
  "mingw-w64-x86_64-ffmpeg"   # Webots movies
  "mingw-w64-x86_64-dlfcn"    # dependency of ffmpeg
)

declare -a OPTIONAL_PACKAGES=(
  "pacman-contrib"            # Distribution script (pactree)
  "swig"                      # Python and Java API wrappers
  "mingw-w64-x86_64-libssh"   # Robotis OP2 robot window
  "mingw-w64-x86_64-libzip"   # Robotis OP2 robot window
  "mingw-w64-x86_64-boost"    # to recompile ROS controller
  "mingw-w64-x86_64-cmake"    # Thymio II dashel library
  "mingw-w64-x86_64-protobuf" # Robocup player controller
  "mingw-w64-i686-gcc"        # libController (32 bit)
)

declare -a DEVELOPMENT_PACKAGES=(
  "mingw-w64-x86_64-clang"    # coding style tests
  "mingw-w64-x86_64-cppcheck" # coding style tests
  "mingw-w64-x86_64-gdb"      # debugging
  "diffutils"                 # cmp and diff utilities
  "procps"                    # pgrep
)

if [ "$1" == "--dev" ]; then
  declare -a PACKAGES=("${BASE_PACKAGES[@]}" "${OPTIONAL_PACKAGES[@]}" "${DEVELOPMENT_PACKAGES[@]}")
elif [ "$1" == "--all" ]; then
  declare -a PACKAGES=("${BASE_PACKAGES[@]}" "${OPTIONAL_PACKAGES[@]}")
else
  declare -a PACKAGES=("${BASE_PACKAGES[@]}")
fi

for i in "${PACKAGES[@]}"
do
  if ! pacman -Q $i 2> /dev/null
  then
    echo $i
    pacman -S --noconfirm $i
  fi
done

script_name=$0
script_full_path=$(dirname "$0")
$script_full_path/qt_windows_installer.sh
