#!/bin/bash

if [ $# -eq 0 ]; then
export PATH=/mingw64/bin:/usr/bin:/c/WINDOWS/system32
cd ~
else
WINDOWS_DRIVE=${1:1:1}:\\${1:3}
export WEBOTS_HOME=${WINDOWS_DRIVE////\\}
PATH="/c/WINDOWS/system32:/c/WINDOWS"
if [ -v PYTHON38_HOME ]; then
PATH+=":$PYTHON38_HOME:$PYTHON38_HOME/Scripts"
elif [ -v PYTHON39_HOME ]; then
PATH+=":$PYTHON39_HOME:$PYTHON39_HOME/Scripts"
elif [ -v PYTHON37_HOME ]; then
PATH+=":$PYTHON37_HOME:$PYTHON37_HOME/Scripts"
fi
PATH+=":$1/msys64/mingw64/bin:/mingw64/bin:/usr/bin"
if [ -v JAVA_HOME ]; then
PATH+=":$JAVA_HOME/bin"
fi
if [ -v MATLAB_HOME ]; then
PATH+=":$MATLAB_HOME/bin"
fi
export PATH=$PATH
cd "$1"
fi
