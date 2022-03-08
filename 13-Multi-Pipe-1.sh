source "${TEST_LIB}/funcs.bash"

run_timeout=15

script=$(cat <<EOM
cat /etc/passwd | sort | sed s/:.*//
echo -----
cat /etc/passwd | sort | sed s/:.*// | grep $(whoami)
echo -----
cat /etc/passwd | sort | grep s | sed s/:.*// | tail -n 3 | head -n 2
echo -----
echo hi | cat | cat | cat | cat | sed s/h/y/g | sed s/i/o/g | shuf
EOM
)

reference_run sh <(echo "${script}") 2> /dev/null

test_start "Long pipeline (several pipes in a single command)"

# ---------- Test Script ----------
echo "${script}"
# -------------- End --------------

run ./$SHELL_NAME < <(echo "${script}")

compare_outputs

test_end
