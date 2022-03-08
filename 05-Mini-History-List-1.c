#if 0
    source "${TEST_LIB}/crunner" -lshell -D_GNU_SOURCE -ldl
#endif

#include <libgen.h>
#include <limits.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "history.h"

void rand_str(char *str, size_t sz)
{
    if (sz == 0) {
        return;
    }

    for (int i = 0; i < sz - 1; ++i){
        if (i == 0) {
            str[i] = 'a' + rand() % 26;
            continue;
        }

        int symb_type = rand() % 4;
        switch (symb_type) {
            case 0:
                str[i] = ' ';
                break;

            case 1:
            case 2:
                str[i] = 'a' + rand() % 26;
                break;

            case 3:
                str[i] = 'A' + rand() % 26;
                break;
        }
    }
    str[sz - 1] = '\0';
}

test_start("Tiny History");

subtest("Tests many strings against a small 6-element history list",
{
    // Determine the stack size so we can allocate more
    struct rlimit rlp;
    getrlimit(RLIMIT_STACK, &rlp);
    char *buf = calloc(1, rlp.rlim_cur);

    hist_init(6);

    for (int i = 0; i < 1001110; ++i) {
        size_t cmd_sz = (rand() % 78) + 2;
        rand_str(buf, cmd_sz);
        hist_add(buf);
    }

    // Add a couple massive strings
    rand_str(buf, rlp.rlim_cur);
    hist_add(buf);

    rand_str(buf, rlp.rlim_cur);
    hist_add(buf);


    // Now add 6 more. Captures the target output to a string.
    char target[81 * 6 + 1] = { 0 };
    char *t_p = target;
    for (int i = 0; i < 6; ++i) {
        size_t cmd_sz = (rand() % 78) + 2;
        rand_str(buf, cmd_sz);
        hist_add(buf);
        t_p += sprintf(t_p, "%d %s\n", i + 1001110 + 3, buf);
    }


    FILE *f1 = tmpfile();
    FILE *f2 = tmpfile();

    pid_t child = fork();
    if (child == 0) {
        dup2(fileno(f1), fileno(stdout));
        hist_print();
        fclose(f1);
        exit(0);
    } else {
        wait(NULL);
    }

    child = fork();
    if (child == 0) {
        dup2(fileno(f2), fileno(stdout));
        printf("%s", target);
        fclose(f2);
        exit(0);
    } else {
        wait(NULL);
    }


    fseek(f1, 0, SEEK_SET);
    fseek(f2, 0, SEEK_SET);

    printf("Program output shown on left, target (expected) output on right.\n\n");
    fflush(stdout);

    char command[1024];
    sprintf(command, "sdiff --expand-tabs --ignore-space-change "
            "/dev/fd/%d /dev/fd/%d",
            fileno(f1),
            fileno(f2));

    int return_value = system(command);
    printf("\n\nIf the outputs match the return value should be 0.");
    test_assert(return_value == 0);

    hist_destroy();
});

test_end
