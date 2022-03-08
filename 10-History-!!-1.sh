source "${TEST_LIB}/funcs.bash"

run_timeout=5

script=$(cat <<EOM
ls -1 -t -r /
!!
uname -a
!!
oeiwiowejweo
!!
echo Goodbye
!!

history
!!
EOM
)

HISTFILE=/dev/null HISTSIZE=100 reference_run \
    bash <(cat "${TEST_DIR}/inputs/bashrc_history"; echo "${script}") 2> /dev/null

test_start "History Execution !!"

# ---------- Test Script ----------
echo "${script}"
# -------------- End --------------

run ./$SHELL_NAME < <(echo "${script}")

compare_outputs --ignore-all-space

test_end
