source "${TEST_LIB}/funcs.bash"

run_timeout=3

fd_check='
{
    if (/==[0-9]*==\s*Open file descriptor/) {
        fd=$0;
        getline;
        if (! /inherited/) {
            print fd
        }
    }
}' 

script=$(cat <<EOM
ls /
cd
ls /
asdfghjklqprewopiqwualasdf # Bad Command!
# Comment only
pwd
EOM
)

test_start "Basic Memory Leak Check"

# ---------- Test Script ----------
echo "${script}"
# -------------- End --------------

program_output=$(timeout ${run_timeout} valgrind \
    --trace-children=no \
    --child-silent-after-fork=yes \
    --leak-check=full \
    --track-fds=yes \
    --track-origins=yes \
    --show-leak-kinds=all \
    ./$SHELL_NAME < <(echo "${script}") 2>&1)

echo "${program_output}"

# Check for open FDs
awk "${fd_check}" <<< "${program_output}" \
    | grep -i '==[0-9]*==.*file descriptor' && test_end 1

# Make sure no leaks are possible
grep -i '==[0-9]*==.*no leaks are possible' \
    <<< "${program_output}" || test_end 1

# If none of the conditions were triggered above, the test passes.
test_end 0 
