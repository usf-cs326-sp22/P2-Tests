source "${TEST_LIB}/funcs.bash"

run_timeout=5

script=$(cat <<EOM
${TEST_DIR}/inputs/scripts/sleeper.sh 1 &
${TEST_DIR}/inputs/scripts/sleeper.sh 2 &
${TEST_DIR}/inputs/scripts/sleeper.sh 500 &
${TEST_DIR}/inputs/scripts/sleeper.sh 100 &
${TEST_DIR}/inputs/scripts/sleeper.sh 450 &
${TEST_DIR}/inputs/scripts/sleeper.sh 3
jobs
${TEST_DIR}/inputs/scripts/kill-parent.sh
EOM
)

expected=$(cat <<EOM
sleeper.sh 100
sleeper.sh 450
sleeper.sh 500
EOM
)

test_start "Background job list" \
    "Requires working SIGINT handler to pass this test"

# ---------- Test Script ----------
echo "${script}"
# -------------- End --------------

# If your shell does not support jobs correctly, the following will time out:
run ./$SHELL_NAME < <(echo "${script}")

# Raw output:
echo "${program_output}"
#############

# We process the output before displaying it here.
# We ignore:
# - pids (unique on each system)
# - job numbers
# - & (optional to display in the job list)
processed_output=$(grep -o 'sleeper.sh\s\+[0-9]\+' <<< "${program_output}" \
    | sort -n)

# Don't forget to check the comments above to understand how
# this output is formatted!
compare <(echo "${expected}") <(echo "${processed_output}")

test_end
