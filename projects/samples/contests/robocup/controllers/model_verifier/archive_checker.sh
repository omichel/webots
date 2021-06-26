#!/bin/bash

function error {
    echo "ERROR: $1" >&2
}

if [[ -z "${WEBOTS_HOME}" ]]; then
    error "Environment variable 'WEBOTS_HOME' is not defined"
    exit 1
fi

ROBOCUP_PATH=${WEBOTS_HOME}/projects/samples/contests/robocup
MODEL_CHECKER=${ROBOCUP_PATH}/controllers/model_verifier/model_checker.sh


if [[ $# -lt 1 ]]
then
    error "Usage: $0 <robot_archive_1.zip> <robot_archive_2.zip> <...>"
    exit 1
fi

for ARCHIVE in $@
do
    echo "Analyzing $ARCHIVE"

    # Folder should be inside the protos folder to be accessible for webots
    TMP_FOLDER=${ROBOCUP_PATH}/protos/tmp

    rm -rf ${TMP_FOLDER}
    unzip -d $TMP_FOLDER $ARCHIVE >/dev/null
    if [[ ! -d ${TMP_FOLDER}/documentation ]]
    then
        error "Missing documentation folder"
    fi

    for robot in $(ls -d ${TMP_FOLDER}/*/ | grep -v documentation)
    do
        ROBOT=$(basename $robot)
        echo "Checking content of robot: $ROBOT"
        ROBOT_FOLDER=${TMP_FOLDER}/${ROBOT}
        EXPECTED_PROTO=${ROBOT_FOLDER}/${ROBOT}.proto
        EXPECTED_POSTURES=${ROBOT_FOLDER}/postures.json

        if [[ ! -f $EXPECTED_POSTURES ]]
        then
            error "Missing file for robot $ROBOT: $EXPECTED_POSTURES"
        fi
        if [[ ! -f $EXPECTED_PROTO ]]
        then
            error "Missing file for robot $ROBOT: $EXPECTED_PROTO"
        else
            ${MODEL_CHECKER} $EXPECTED_PROTO
        fi
    done
done
