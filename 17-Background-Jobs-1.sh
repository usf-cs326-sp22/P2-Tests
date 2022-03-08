source "${TEST_LIB}/funcs.bash"

run_timeout=2

rn=${RANDOM}

script=$(cat <<EOM
${TEST_DIR}/inputs/scripts/sleeper.sh 500 &
${TEST_DIR}/inputs/scripts/sleeper.sh 100 &
echo Your lucky number is ${rn}
${TEST_DIR}/inputs/scripts/sleeper.sh 85 &
${TEST_DIR}/inputs/scripts/sleeper.sh 450 &
${TEST_DIR}/inputs/scripts/kill-parent.sh
EOM
)

test_start "Background job support"

# ---------- Test Script ----------
echo "${script}"
# -------------- End --------------

# If your shell does not support jobs correctly, the following will time out:
run ./$SHELL_NAME < <(echo "${script}")

compare <(echo "Your lucky number is ${rn}") <(echo "${program_output}")

test_end
