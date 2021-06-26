#!/bin/bash

function error {
    echo "ERROR: $1" >&2
}

if [[ -z "${WEBOTS_HOME}" ]]; then
    error "Environment variable 'WEBOTS_HOME' is not defined"
    exit 1
fi

if [[ $# -lt 1 ]]
then
    echo 2>&1 "Usage: $0 [--no-display] <robot_model.proto>"
    exit 1
fi

# Parsing args, see: https://stackoverflow.com/a/14203146
POSITIONAL=()
while [[ $# -gt 0 ]]
do
    key="$1"
    if [ $key == "--no-display" ]
    then
        NO_DISPLAY=TRUE
        shift
    else
        POSITIONAL+=("$1")
        shift
    fi
done

set -- "${POSITIONAL[@]}" # restore positional parameters

if [ ! -f $1 ]
then
    echo "$1 is not a file"
    exit 1
fi

export ROBOT_PATH=$(realpath $1)
export ROBOT_NAME=$(basename $1 | sed s/.proto// )
ROBOCUP_PATH=${WEBOTS_HOME}/projects/samples/contests/robocup
MV_PATH=${ROBOCUP_PATH}/controllers/model_verifier
RESULTS_FOLDER=results/$ROBOT_NAME
MARKDOWN_SRC=${MV_PATH}/report.md
SUBMISSION_SRC=${MV_PATH}/robot_properties.json
MV_LOG_SRC=${MV_PATH}/model_verifier.log
SIMULATOR_OUT=$RESULTS_FOLDER/simulator.log
SIMULATOR_ERR=$RESULTS_FOLDER/simulator.err
MARKDOWN_DST=${RESULTS_FOLDER}/report.md
MV_LOG_DST=${RESULTS_FOLDER}/model_verifier.log
SUBMISSION_DST=${RESULTS_FOLDER}/robot_properties.json
REPORT_DST=$RESULTS_FOLDER/report.pdf
MAX_LINE_WIDTH=120

if [[ $NO_DISPLAY == TRUE ]]
then
    export MODEL_VERIFIER_NO_DISPLAY="TRUE"
else
    unset MODEL_VERIFIER_NO_DISPLAY
fi

echo "Checking robot $ROBOT_NAME in $ROBOT_PATH"
mkdir -p $RESULTS_FOLDER
$WEBOTS_HOME/webots --stdout --stderr --batch ${ROBOCUP_PATH}/worlds/model_verifier.wbt \
                    > $SIMULATOR_OUT 2> $SIMULATOR_ERR

# Adding simulator errors to markdown file
if [[ $( wc -l < ${SIMULATOR_ERR} ) -gt 0 ]]
then
    printf "\n# Simulator errors\n" >> $MARKDOWN_SRC
    printf '```\n' >> $MARKDOWN_SRC
    fmt -st --width $MAX_LINE_WIDTH $SIMULATOR_ERR >> $MARKDOWN_SRC
    printf '\n```\n' >> $MARKDOWN_SRC
fi

# Adding model verification log
if [[ $( wc -l < ${MV_LOG_SRC} ) -gt 0 ]]
then
    printf "\n# Model verifier logs\n" >> $MARKDOWN_SRC
    printf '```\n' >> $MARKDOWN_SRC
    fmt -st --width $MAX_LINE_WIDTH $MV_LOG_SRC >> $MARKDOWN_SRC
    printf '\n```\n' >> $MARKDOWN_SRC
fi

# Moving files
mv ${MARKDOWN_SRC} $MARKDOWN_DST
mv ${MV_LOG_SRC} ${MV_LOG_DST}
mv ${SUBMISSION_SRC} ${SUBMISSION_DST}

# Producing report
pandoc -V geometry:landscape -V geometry:margin=2cm $MARKDOWN_DST -o $REPORT_DST
