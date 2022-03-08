source "${TEST_LIB}/funcs.bash"

run_timeout=5

reference_run bash < "${TEST_DIR}/inputs/commands.sh" 2> /dev/null

test_start "Basic Execution" \
    "Basic process execution both from PATH and with absolute path.  Inputted directly to the shell via input redirection."

run ./$SHELL_NAME < "${TEST_DIR}/inputs/commands.sh"

compare_outputs

test_end
