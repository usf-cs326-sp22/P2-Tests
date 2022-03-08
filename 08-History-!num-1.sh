source "${TEST_LIB}/funcs.bash"

run_timeout=5

HISTFILE=/dev/null HISTSIZE=100  reference_run \
    bash <(cat "${TEST_DIR}/inputs/bashrc_history" "${TEST_DIR}/inputs/history-script-3.sh") 2> /dev/null

test_start "History Execution !num"

run ./$SHELL_NAME < "${TEST_DIR}/inputs/history-script-3.sh"

compare_outputs --ignore-all-space

test_end
