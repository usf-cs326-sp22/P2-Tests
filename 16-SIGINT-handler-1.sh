source "${TEST_LIB}/funcs.bash"

run_timeout 5

trap '#^C Killed me!' INT

script=$(cat <<EOM
${TEST_DIR}/inputs/scripts/kill-parent.sh
sleep 1
echo The shell is still running!
EOM
)

test_start "^C should be ignored by the shell" \
    "NOTE: this test runs in scripting mode; make sure the prompt is not printed by SIGINT handler"

run ./$SHELL_NAME < <(echo "${script}")

compare <(echo "The shell is still running!") <(echo "${program_output}")

test_end
