#!/bin/bash

# WARNING: running this shell changes existing files in the webots directory

function error {
    echo "ERROR: $1" >&2
}

# Parsing args, see: https://stackoverflow.com/a/14203146
POSITIONAL=()
while [[ $# -gt 0 ]]
do
    key="$1"
    if [ $key == "--no-rendering" ]
    then
        NO_RENDERING=TRUE
        shift
    else
        POSITIONAL+=("$1")
        shift
    fi
done

set -- "${POSITIONAL[@]}" # restore positional parameters

if [[ $# -lt 1 || ! -d $1 ]]
then
    error "Usage: $0 <team_folder>"
    if [[ $# -gt 0 ]]
    then
        error "$1 is not a folder"
    fi
    exit 1
fi

TEAM_FOLDER=$1
TEAM_CONFIG=${TEAM_FOLDER}/team.json
ROBOCUP_PATH=${WEBOTS_HOME}/projects/samples/contests/robocup
REF_PATH=${ROBOCUP_PATH}/controllers/referee
PLAYER_PATH=${ROBOCUP_PATH}/controllers/player
CLIENT=${PLAYER_PATH}/client
ROBOCUP_WORLD=${ROBOCUP_PATH}/worlds/robocup.wbt


if [[ ! -f ${TEAM_CONFIG} ]]
then
    error "$TEAM_CONFIG not found (or is not a file)"
    exit 1
fi

ROBOT_FOLDERS=$(ls -d ${TEAM_FOLDER}/*/ | grep -v documentation)

if [[ ${#ROBOT_FOLDERS[@]} != 1 ]]
then
    error "Currently only teams with one robot are supported"
    exit 1
fi

if [[ $NO_RENDERING == TRUE ]]
then
    SUFFIX="without_rendering"
else
    SUFFIX="with_rendering"
fi

LOG_PERF="$(basename $TEAM_FOLDER)_${SUFFIX}.perf"
ROBOT_FOLDER=${ROBOT_FOLDERS[0]}
POSTURES_FILE=${ROBOT_FOLDER}/postures.json

# Crafting a request message for robot clients
ACTUATOR_REQUEST_PATH=actuator_requests.txt
JOINT_NAME="$(grep -A 1 upright ${POSTURES_FILE} | tail -n 1 | awk 'BEGIN { FS = "\"" } ; { print $2 }')"
printf "motor_positions {\n  name: \"%s\"\n  position: 0.0\n}" "$JOINT_NAME" > $ACTUATOR_REQUEST_PATH

# Adding robot to robocup project
cp -r "$ROBOT_FOLDER" "${ROBOCUP_PATH}/protos"

# Replacing team files
for dst in team_1.json team_2.json
do
    cp "$TEAM_CONFIG" "${REF_PATH}/$dst"
done

rm -rf "${WEBOTS_HOME}/${LOG_PERF}"
WEBOTS_OPT=("--batch" "--sysinfo" "--stdout" "--stderr" "--minimize" "--log-performance=$LOG_PERF")
if [[ $NO_RENDERING == "TRUE" ]]
then
    WEBOTS_OPT+=("--no-rendering")
fi
$WEBOTS_HOME/webots ${WEBOTS_OPT[@]} $ROBOCUP_WORLD >${TEAM_FOLDER}/${SUFFIX}.log 2>${TEAM_FOLDER}/${SUFFIX}.err &

echo "Waiting warm-up before trying to connect clients"
sleep 80

for team in RED BLUE
do
    for player in 1 2 3 4
    do
        port=10000
        if [[ $team == "BLUE" ]]
        then
            let port+=20
        fi
        let port+=player
        OUT="${TEAM_FOLDER}/${team}_${player}_${SUFFIX}.txt"
        echo "Starting $team $player on port $port, output to $OUT"
        stdbuf -oL $CLIENT 127.0.0.1 $port &>$OUT &
    done
done

wait

tail -n 7 ${TEAM_FOLDER}/*${SUFFIX}.txt
