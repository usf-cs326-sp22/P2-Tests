source "${TEST_LIB}/funcs.bash"

run_timeout=5

run_scripts() {
    fail=0

    for script in "${TEST_DIR}"/inputs/basic-builtins/*.sh; do
        echo "-> Running script: ${script}"
        echo
        echo "Script Contents: ----------------------------------"
        cat "${script}"
        echo "---------------------------------------------------"
        echo

        reference_run sh "${script}" 2> /dev/null
        run ./$SHELL_NAME < <(cat "${script}")
        echo; echo

        echo "-> Comparing outputs"
        compare_outputs || fail=${?}
        echo; echo
    done
    return ${fail}
}

test_start "Basic Builtins (cd, comments, exit)"

run_scripts

test_end
