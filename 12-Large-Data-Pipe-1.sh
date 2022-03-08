source "${TEST_LIB}/funcs.bash"

run_timeout=15

script=$(cat <<EOM
seq 10000000 | wc -l
EOM
)

reference_run sh <(echo "${script}") 2> /dev/null

test_start "Pipe with a large amount of lines"

# ---------- Test Script ----------
echo "${script}"
# -------------- End --------------

run ./$SHELL_NAME < <(echo "${script}")

compare_outputs

test_end
