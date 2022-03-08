source "${TEST_LIB}/funcs.bash"

run_timeout=30

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

test_start "Thorough Memory Leak Check"

program_output=$(valgrind \
    --trace-children=no \
    --child-silent-after-fork=yes \
    --leak-check=full \
    --track-fds=yes \
    --track-origins=yes \
    --show-leak-kinds=all \
    ./$SHELL_NAME < "${TEST_DIR}/inputs/leak.sh" 2>&1)
echo "${program_output}"

# Check for open FDs
awk "${fd_check}" <<< "${program_output}" \
    | grep -i '==[0-9]*==.*file descriptor' && test_end 1

# Make sure no leaks are possible
grep -i '==[0-9]*==.*no leaks are possible' \
    <<< "${program_output}" || test_end 1

# If none of the conditions were triggered above, the test passes.
test_end 0 
