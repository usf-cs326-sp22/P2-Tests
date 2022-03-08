#if 0
    source "${TEST_LIB}/crunner" -lshell -D_GNU_SOURCE -ldl
#endif

#include <libgen.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "ui.h"

char *command_generator(const char *text, int state);

#define MAX_MATCHES 1024

static int comparator(const void *a, const void *b)
{
    return strcmp(*(char **) a, *(char **) b);
}

test_start("Autocomplete functionality");

subtest("Nothing in PATH",
{
    setenv("PATH", "", 1);
    init_ui();
    const char *str = "this_is_a_test";

    char *match = command_generator(str, 0);
    test_assert(match == NULL);
});

subtest("Nothing in PATH, blank input string",
{
    setenv("PATH", "", 1);
    init_ui();
    const char *str = "";

    int match_count = 0;
    char *match;
    char *matches[MAX_MATCHES] = { NULL };
    while ((match = command_generator(str, match_count)) != NULL) {
        matches[match_count++] = match;
    }

    test_assert(match_count == 4);
    printf("> match_count = '%d'\n", match_count);

    /* Sort the list of matches */
    qsort(matches, match_count, sizeof(const char *), comparator);

    char *expected_matches[] = {
        "exit",
        "jobs",
        "history",
        "cd",
    };
    qsort(expected_matches, match_count, sizeof(const char *), comparator);

    for (int i = 0; i < match_count; ++i) {
        printf("\n> matches[%d] = '%s'\n", i, matches[i]);
        test_assert_str(matches[i], "==", expected_matches[i], 50);
    }
});

subtest("Single directory in PATH, 'v' entered",
{
    char *test_dir = getenv("TEST_DIR");
    char *input_dir = "/inputs/fakepath/";

    /* we'll add an extra char here for the subdirectories in fakepath */
    char *fake_path = malloc(strlen(test_dir) + strlen(input_dir) + 2);
    strcpy(fake_path, test_dir);
    strcat(fake_path, input_dir);
    strcat(fake_path, "a");

    setenv("PATH", fake_path, 1);
    init_ui();
    const char *str = "v";

    int match_count = 0;
    char *match;
    char *matches[MAX_MATCHES] = { NULL };
    while ((match = command_generator(str, match_count)) != NULL) {
        matches[match_count++] = match;
    }

    test_assert(match_count == 6);
    printf("> match_count = '%d'\n", match_count);

    /* Sort the list of matches */
    qsort(matches, match_count, sizeof(const char *), comparator);

    char *expected_matches[] = {
        "v",
        "vim",
        "vimdiff",
        "vim9000",
        "vix",
        "very_very_cool"
    };
    qsort(expected_matches, match_count, sizeof(const char *), comparator);

    for (int i = 0; i < match_count; ++i) {
        printf("\n> matches[%d] = '%s'\n", i, matches[i]);
        test_assert_str(matches[i], "==", expected_matches[i], 50);
    }
});

subtest("Single directory in PATH, '_' entered",
{
    char *test_dir = getenv("TEST_DIR");
    char *input_dir = "/inputs/fakepath/";

    /* we'll add an extra char here for the subdirectories in fakepath */
    char *fake_path = malloc(strlen(test_dir) + strlen(input_dir) + 2);
    strcpy(fake_path, test_dir);
    strcat(fake_path, input_dir);
    strcat(fake_path, "a");

    setenv("PATH", fake_path, 1);
    init_ui();
    const char *str = "_";

    int match_count = 0;
    char *match;
    char *matches[MAX_MATCHES] = { NULL };
    while ((match = command_generator(str, match_count)) != NULL) {
        matches[match_count++] = match;
    }

    test_assert(match_count == 2);

    /* Sort the list of matches */
    qsort(matches, match_count, sizeof(const char *), comparator);

    char *expected_matches[] = {
        "_",
        "________test_hi",
    };
    qsort(expected_matches, match_count, sizeof(const char *), comparator);

    for (int i = 0; i < match_count; ++i) {
        printf("\n> matches[%d] = '%s'\n", i, matches[i]);
        test_assert_str(matches[i], "==", expected_matches[i], 50);
    }
});

subtest("Invalid directories in PATH, 'hello' entered",
{
    char *test_dir = getenv("TEST_DIR");
    char *input_dir = "/inputs/fakepath/";

    /* we'll add an extra char here for the subdirectories in fakepath */
    char *fake_path1 = malloc(strlen(test_dir) + strlen(input_dir) + 2);
    strcpy(fake_path1, test_dir);
    strcat(fake_path1, input_dir);
    strcat(fake_path1, "z");

    size_t len = strlen(fake_path1);

    char *fake_path2 = strdup(fake_path1);
    fake_path2[len - 1] = 'm';

    char *fake_path3 = strdup(fake_path1);
    fake_path3[len - 1] = 'x';

    char *fake_path4 = strdup(fake_path1);
    fake_path4[len - 1] = 'c';

    size_t new_len = strlen(fake_path1)
            + strlen(fake_path2)
            + strlen(fake_path3)
            + strlen(fake_path4)
            + 4;

    char *new_path = malloc(new_len * sizeof(*new_path));

    snprintf(new_path, new_len, "%s:%s:%s:%s",
            fake_path1, fake_path2, fake_path3, fake_path4);

    setenv("PATH", new_path, 1);
    init_ui();
    const char *str = "hello";

    int match_count = 0;
    char *match;
    char *matches[MAX_MATCHES] = { NULL };
    while ((match = command_generator(str, match_count)) != NULL) {
        matches[match_count++] = match;
    }

    test_assert(match_count == 1);
    if (match_count == 1) {
        test_assert_str(matches[0], "==", "hellooooooo                     ...world!", 50);
    }
});

subtest("Multiple directories in PATH, 'hi' entered",
{
    char *test_dir = getenv("TEST_DIR");
    char *input_dir = "/inputs/fakepath/";

    /* we'll add an extra char here for the subdirectories in fakepath */
    char *fake_path1 = malloc(strlen(test_dir) + strlen(input_dir) + 2);
    strcpy(fake_path1, test_dir);
    strcat(fake_path1, input_dir);
    strcat(fake_path1, "c");

    size_t len = strlen(fake_path1);

    char *fake_path2 = strdup(fake_path1);
    fake_path2[len - 1] = 'd';

    size_t new_len = strlen(fake_path1)
            + strlen(fake_path2)
            + 2;

    char *new_path = malloc(new_len * sizeof(*new_path));
    snprintf(new_path, new_len, "%s:%s", fake_path1, fake_path2);

    setenv("PATH", new_path, 1);
    init_ui();
    const char *str = "hi";

    int match_count = 0;
    char *match;
    char *matches[MAX_MATCHES] = { NULL };
    while ((match = command_generator(str, match_count)) != NULL) {
        matches[match_count++] = match;
    }

    test_assert(match_count == 4);

    /* Sort the list of matches */
    qsort(matches, match_count, sizeof(const char *), comparator);
    char *expected_matches[] = {
        "hi_there_______________:-)",
        "history",
        "hidutil",
        "hiutil",
    };
    qsort(expected_matches, match_count, sizeof(const char *), comparator);

    for (int i = 0; i < match_count; ++i) {
        printf("\n> matches[%d] = '%s'\n", i, matches[i]);
        test_assert_str(matches[i], "==", expected_matches[i], 50);
    }
});


subtest("PATH spans many files, 'zi' entered",
{
    char *test_dir = getenv("TEST_DIR");
    char *input_dir = "/inputs/fakepath/";

    /* we'll add an extra char here for the subdirectories in fakepath */
    char *fake_path1 = malloc(strlen(test_dir) + strlen(input_dir) + 2);
    strcpy(fake_path1, test_dir);
    strcat(fake_path1, input_dir);
    strcat(fake_path1, "a");

    size_t len = strlen(fake_path1);

    char *fake_path2 = strdup(fake_path1);
    fake_path2[len - 1] = 'b';

    char *fake_path3 = strdup(fake_path1);
    fake_path3[len - 1] = 'c';

    char *fake_path4 = strdup(fake_path1);
    fake_path4[len - 1] = 'd';

    size_t new_len = strlen(fake_path1)
            + strlen(fake_path2)
            + strlen(fake_path3)
            + strlen(fake_path4)
            + 4;

    char *new_path = malloc(new_len * sizeof(*new_path));

    snprintf(new_path, new_len, "%s:%s:%s:%s",
            fake_path1, fake_path2, fake_path3, fake_path4);

    printf("path: %s\n", new_path);
    setenv("PATH", new_path, 1);
    init_ui();
    const char *str = "zi";

    int match_count = 0;
    char *match;
    char *matches[MAX_MATCHES] = { NULL };
    while ((match = command_generator(str, match_count)) != NULL) {
        matches[match_count++] = match;
    }

    test_assert(match_count == 12);

    /* Sort the list of matches */
    qsort(matches, match_count, sizeof(const char *), comparator);

    char *expected_matches[] = {
        "zit",
        "zic",
        "zii",
        "zip",
        "zipcloak",
        "zipdetails",
        "zipdetails5.18",
        "zipdetails5.28",
        "zipgrep",
        "zipinfo",
        "zipnote",
        "zipsplit"
    };
    qsort(expected_matches, match_count, sizeof(const char *), comparator);

    for (int i = 0; i < match_count; ++i) {
        printf("\n> matches[%d] = '%s'\n", i, matches[i]);
        test_assert_str(matches[i], "==", expected_matches[i], 50);
    }
});

test_end
