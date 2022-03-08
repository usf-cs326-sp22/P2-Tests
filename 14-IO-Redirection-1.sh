source "${TEST_LIB}/funcs.bash"

run_timeout=5

script=$(cat <<EOM
rm -f ${TEST_DIR}/final_output.txt
rm -f ${TEST_DIR}/testfile1 ${TEST_DIR}/testfile2 ${TEST_DIR}/testfile3 ${TEST_DIR}/testfile4

echo Hello world this is a nice long message to save to a file > ${TEST_DIR}/testfile1
cat /etc/passwd > ${TEST_DIR}/testfile2
echo And here is another message saved to a different file > ${TEST_DIR}/testfile3

sort < /etc/passwd > ${TEST_DIR}/testfile4
sort >> ${TEST_DIR}/testfile4 < /etc/hostname
echo We just printed the password file in sorted >> ${TEST_DIR}/testfile4
echo order followed by hostname. >> ${TEST_DIR}/testfile4

echo This will overwrite the first message > ${TEST_DIR}/testfile1
echo But this message will not overwrite >> ${TEST_DIR}/testfile3
echo In fact, it will append >> ${TEST_DIR}/testfile3

cat ${TEST_DIR}/testfile1 ${TEST_DIR}/testfile2 ${TEST_DIR}/testfile3 ${TEST_DIR}/testfile4 > ${TEST_DIR}/final_output.txt
cat ${TEST_DIR}/final_output.txt

${TEST_DIR}/inputs/scripts/get-stats.sh ${TEST_DIR}/final_output.txt

rm -f ${TEST_DIR}/final_output.txt
rm -f ${TEST_DIR}/testfile1 ${TEST_DIR}/testfile2 ${TEST_DIR}/testfile3 ${TEST_DIR}/testfile4
EOM
)

reference_run sh <(echo "${script}") 2> /dev/null

test_start "IO redirection" \
"This test writes several strings to files and ensures their results are
correct using both input and output redirection. It also checks to make sure
the permissions are set up correctly with open() -- use 0666."

# ---------- Test Script ----------
echo "${script}"
# -------------- End --------------

run ./$SHELL_NAME < <(echo "${script}")

compare_outputs

test_end
