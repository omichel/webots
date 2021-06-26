#!/bin/bash

# Usage:
# --print-only: Only print the cached results
# --lazy: Only run uncached tests or cached but unsuccessful tests

PRINT_ONLY=false
LAZY=false
if [ $# -gt 0 ]
then
    if [ $1 == "--print-only" ]
    then
        PRINT_ONLY=true
    elif [ "$1" == "--lazy" ]
    then
        LAZY=true
    else
        echo "Usage: $0 [--print-only] [--lazy]"
        exit -1
    fi
fi

script_dir=$(dirname "$(realpath -s "$0")")
source "$script_dir/common.sh"

assert_env_vars

parse_log_file() {
    local test_log=$1
    local -n nb_success_=$2
    local -n nb_tests_=$3

    local result_line=$(awk '/TEST RESULTS/ { print $3 }' "$test_log")
    nb_success_=$(echo "$result_line" | awk 'BEGIN { FS = "/" } ; { print $1 }')
    nb_tests_=$(echo "$result_line" | awk 'BEGIN { FS = "/" } ; { print $2 }')
}

# When a test is launched but its execution is aborted (e.g. via ctrl+c), the
# log file might not contain a test result.
log_file_contains_test_result() {
    local test_log=$1
    local nb_success
    local nb_tests
    parse_log_file "$test_log" nb_success nb_tests

    [[ -f "$test_log" && "$nb_success" && "$nb_tests" ]]
}

should_run_tests() {
    local test_log=$1

    if [ "$PRINT_ONLY" = true ]; then
        # don't run the test
        return 1
    fi

    if [ $LAZY = true ]; then
      if ! log_file_contains_test_result "$test_log"; then
          # run the test
          return 0
      else
          parse_log_file "$test_log" nb_success nb_tests

          # run the test if some tests were unsuccessful
          [ "$nb_success" -lt "$nb_tests" ]
          return
      fi
    fi

    # run the test, if no command line option is set
    return 0
}

readarray -d '' TEST_FILES < <(find . -name "test_scenario.json" -print0 | sort --zero-terminated --human-numeric-sort)

TOT_SUCCESS=0
TOT_TESTS=0
NTH_TEST=0

COLOR_RED='\e[0;31m'
COLOR_GREEN='\e[0;32m'
COLOR_RESET='\e[0m'

for test_file in "${TEST_FILES[@]}"
do
    ((NTH_TEST+=1))

    folder=$(dirname ${test_file})
    test_log="${folder}/test.log"
    referee_log="${folder}/referee.log"
    msg_prefix="[$NTH_TEST/${#TEST_FILES[@]}] ${folder:2}"

    printf '%-60.60s' "$msg_prefix"
    if should_run_tests "$test_log"
    then
        # On Windows the "| tee" is needed to ensure the output of the
        # launch_test.sh is flushed and fully written in the output file.
        # Using a standard redirection ">" is unfortunately not sufficient
        ./launch_test.sh ${folder} | tee ${test_log} > /dev/null
        cp ../log.txt ${referee_log}
    fi

    declare NB_SUCCESS
    declare NB_TESTS
    parse_log_file "$test_log" NB_SUCCESS NB_TESTS

    if ! log_file_contains_test_result "$test_log"
    then
        printf "$COLOR_RED %s %s$COLOR_RESET\n" FAIL "Log contains no test result. Maybe the test was aborted?"
        continue
    elif [ $NB_SUCCESS -lt $NB_TESTS ]
    then
        printf "$COLOR_RED%s %d/%d$COLOR_RESET\n" FAIL $NB_SUCCESS $NB_TESTS
    else
        printf "$COLOR_GREEN%s %d/%d$COLOR_RESET\n" PASS $NB_SUCCESS $NB_TESTS
    fi

    ((TOT_SUCCESS+=NB_SUCCESS))
    ((TOT_TESTS+=NB_TESTS))
done

echo "=========================="
echo $(printf "# GLOBAL RESULTS: %3d/%3d #" $TOT_SUCCESS $TOT_TESTS)
echo "=========================="
