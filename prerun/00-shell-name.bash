
export SHELL_NAME=$(sed -n -e '/^bin/p' "${TEST_DIR}/../Makefile" \
    | sed 's/bin=\(.*\)$/\1/g')
if [[ -z "${SHELL_NAME}" ]]; then
    export SHELL_NAME=crash
fi


