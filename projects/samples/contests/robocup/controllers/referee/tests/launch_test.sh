#!/bin/bash


if [ $# -lt 1 ]
then
    echo "Usage $0 <test_path> [--render]"
    exit -1
fi

WEBOTS_OPTIONS="--stdout --stderr --mode=fast"

if [ $# == 2 ]
then
    if [ $2 != "--render" ]
    then
        echo "Unknown option $2"
        exit -1
    fi
else
    WEBOTS_OPTIONS="${WEBOTS_OPTIONS} --no-rendering --minimize"
fi

script_dir=$(dirname "$(realpath -s "$0")")
source "$script_dir/common.sh"

assert_env_vars

export PYTHONPATH="${WEBOTS_HOME}/projects/samples/contests/robocup/controllers/referee"  # this should be removed once https://github.com/cyberbotics/webots/issues/3011 is fixed
if [ "$(expr substr $(uname -s) 1 10)" == "MSYS_NT-10" ]; then
  WEBOTS="webots"
else
  WEBOTS="${WEBOTS_HOME}/webots"
fi

ROBOCUP_PATH="${WEBOTS_HOME}/projects/samples/contests/robocup"
WEBOTS_WORLD="${ROBOCUP_PATH}/worlds/robocup.wbt"

TEST_FOLDER="$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
TEST_CLIENT="${ROBOCUP_PATH}/controllers/player/test_client"


# Automatically launch test clients if applicable
if [[ -f "${TEST_FOLDER}/clients.txt" ]]
then
    if pgrep test_client; then
        echo >&2 "Error: there are still clients running!"
        exit 1
    fi
    client_id=1
    while read line; do
        ${TEST_CLIENT} $line > ${TEST_FOLDER}/client_${client_id}.log 2>&1 &
        let client_id++
    done < "${TEST_FOLDER}/clients.txt"
fi

WEBOTS_ROBOCUP_TEST_SCENARIO="${TEST_FOLDER}/test_scenario.json" \
                            WEBOTS_ROBOCUP_GAME="${TEST_FOLDER}/game.json" \
                            $WEBOTS $WEBOTS_OPTIONS $WEBOTS_WORLD
