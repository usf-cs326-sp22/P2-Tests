source "${TEST_LIB}/funcs.bash"

run_timeout=5

script=$(cat <<EOM
rm -f ${TEST_DIR}/mega-output.txt
rm -f ${TEST_DIR}/mega-output2.txt
ls -1 /bin | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n-1 | tail -n+2 | head -n 10 | tail -n 5 > ${TEST_DIR}/mega-output.txt
sort -r < ${TEST_DIR}/mega-output.txt > ${TEST_DIR}/mega-output2.txt
cat ${TEST_DIR}/mega-output2.txt >> ${TEST_DIR}/mega-output.txt
cat ${TEST_DIR}/mega-output.txt
rm -f ${TEST_DIR}/mega-output.txt
rm -f ${TEST_DIR}/mega-output2.txt
EOM
)

reference_run sh <(echo "${script}") 2> /dev/null

test_start "Piping and redirecting a whole, whole lot of times..."

# script hidden for brevity -- check the test case file to see the entire thing.

run ./$SHELL_NAME < <(echo "${script}")

compare_outputs

test_end
