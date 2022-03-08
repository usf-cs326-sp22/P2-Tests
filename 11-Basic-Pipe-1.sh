source "${TEST_LIB}/funcs.bash"

run_timeout=5

script=$(cat <<EOM
ls -1 / | sort -r
EOM
)

reference_run sh <(echo "${script}") 2> /dev/null

test_start "Basic pipe support (single pipe)"

# ---------- Test Script ----------
echo "${script}"
# -------------- End --------------

run ./$SHELL_NAME < <(echo "${script}")

compare_outputs

test_end
