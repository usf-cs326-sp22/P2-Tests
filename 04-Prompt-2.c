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

void rand_str(char *str, size_t sz)
{
    for (int i = 0; i < sz - 1; ++i){
        str[i] = 'a' + rand() % 26;
    }
    str[sz - 1] = '\0';
}

int mkdir_r(char *dir, mode_t mode)
{
    if (dir == NULL) {
        return 1;
    }

    struct stat sb;
    if (!stat(dir, &sb))
        return 0;

    mkdir_r(dirname(strdup(dir)), mode);
    return mkdir(dir, mode);
}

test_start("Tests the shell prompt to ensure required elements are present.");

init_ui();
char *prompt = prompt_line();
test_flush();

subtest("Username present in prompt",
{
    printf("Prompt string: %s\n", prompt);
    char *login = cuserid(NULL);
    if (login == NULL) {
        perror("getlogin");
    }
    test_assert(strstr(prompt, login) != NULL);
});

subtest("Hostname present in prompt",
{
    printf("Prompt string: %s\n", prompt);
    char hn[HOST_NAME_MAX + 1];
    gethostname(hn, HOST_NAME_MAX + 1);
    test_assert(strstr(prompt, hn) != NULL);
});

subtest("Absolute Paths",
{
    char *path1 = "/usr/local/bin";
    chdir(path1);
    prompt = prompt_line();
    printf("Prompt: %s\nPath: %s", prompt, path1);
    test_assert(strstr(prompt, path1) != NULL);
    puts("");

    char *path2 = "/etc";
    chdir(path2);
    prompt = prompt_line();
    printf("Prompt: %s\nPath: %s", prompt, path2);
    test_assert(strstr(prompt, path2) != NULL);
    puts("");

    char *path3 = "/var/db";
    chdir(path3);
    prompt = prompt_line();
    printf("Prompt: %s\nPath: %s", prompt, path3);
    test_assert(strstr(prompt, path3) != NULL);
    puts("");
});

subtest("Home Paths",
{
    char *home = getenv("HOME");
    test_assert(home != NULL);

    /* The first one is easy: HOME = ~ */
    chdir(home);
    prompt = prompt_line();
    printf("Prompt: %s\nPath: %s", prompt, home);
    test_assert(strstr(prompt, "~") != NULL);
    puts("");

    /* This creates a randomized directory under $HOME: */
    char path1[PATH_MAX];
    char rand1[10];
    rand_str(rand1, 10);
    snprintf(path1, PATH_MAX, "%s/%s", home, rand1);
    mkdir(path1, 0777);
    chdir(path1);
    prompt = prompt_line();
    printf("Prompt: %s\nPath: %s", prompt, path1);
    char expected[500] = "~/";
    strcat(expected, rand1);
    test_assert(strstr(prompt, expected) != NULL);
    puts("");

    char path2[PATH_MAX];
    char rand2[35];
    rand_str(rand2, 35);
    snprintf(path2, PATH_MAX, "%s/%s/%s", home, rand1, rand2);
    mkdir(path2, 0777);
    chdir(path2);
    prompt = prompt_line();
    printf("Prompt: %s\nPath: %s", prompt, path2);
    snprintf(expected, 500, "~/%s/%s", rand1, rand2);
    test_assert(strstr(prompt, expected) != NULL);
    puts("");

    char path3[PATH_MAX];
    char rand3[15];
    rand_str(rand3, 15);
    snprintf(path3, PATH_MAX, "%s/%s/%s/%s", home, rand1, rand2, rand3);
    mkdir(path3, 0777);
    chdir(path3);
    prompt = prompt_line();
    printf("Prompt: %s\nPath: %s", prompt, path3);
    snprintf(expected, 500, "~/%s/%s/%s", rand1, rand2, rand3);
    test_assert(strstr(prompt, expected) != NULL);
    puts("");

    rmdir(path3);
    rmdir(path2);
    rmdir(path1);

    char path4[PATH_MAX];
    char rand4[5];
    rand_str(rand4, 5);
    snprintf(path4, PATH_MAX, "/tmp%s/%s", home, rand4);
    mkdir_r(path4, 0777);
    chdir(path4);
    prompt = prompt_line();
    printf("Prompt: %s\nPath: %s", prompt, path4);
    test_assert(strstr(prompt, path4) != NULL);
    puts("");
});

test_end
