#if 0
    source "${TEST_LIB}/crunner" -lshell -lreadline -D_GNU_SOURCE -ldl
#endif

#include <libgen.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <readline/readline.h>

#include "ui.h"
#include "history.h"

void press_up(void)
{
    printf("[ UP ]\n");
    fflush(stdout);
    key_up(1, 65);
}

void press_down(void)
{
    printf("[DOWN]\n");
    fflush(stdout);
    key_down(1, 66);
}

//void enter_search(char *str)
//{
//    printf("User typed: [%s]\n", str);
//    rl_replace_line(str, 1);
//}

test_start("History Navigation");

rl_line_buffer = malloc(128);

init_ui();

hist_init(600);

subtest("No commands entered",
{
    press_up();
    press_down();
});

char *test_dir = getenv("TEST_DIR");
char *script = "/inputs/navigation1.sh";
char *str = malloc(strlen(test_dir) + strlen(script) + 1);
strcpy(str, test_dir);
strcat(str, script);

FILE *f = fopen(str, "r");
char *line = NULL;
size_t line_sz = 0;
ssize_t read_sz = 0;
while ((read_sz = getline(&line, &line_sz, f)) != -1) {
    line[read_sz - 1] = '\0';
    hist_add(line);
}

str[strlen(str) - 4] = '2'; // switch to 'navigation2.sh'
int fd = open(str, O_RDONLY);
if (fd == -1) {
    fprintf(stderr, "-> %s\n", str);
    perror("open");
    exit(1);
}
dup2(fd, fileno(stdin));

hist_print();

puts("\n\n");
read_command();

subtest("Blank search",
{
    press_up();
    test_assert_str(rl_line_buffer, "==", "goodbye", 7);
    printf("> rl_line_buffer = '%s'\n", rl_line_buffer);

    press_up();
    press_up();
    press_up();
    test_assert_str(rl_line_buffer, "==", "cat /proc/uptime", 16);
    printf("> rl_line_buffer = '%s'\n", rl_line_buffer);

    press_down();
    test_assert_str(rl_line_buffer, "==", "uptime", 6);
    printf("> rl_line_buffer = '%s'\n", rl_line_buffer);

    press_down();
    press_down();
    press_down();
    test_assert_str(rl_line_buffer, "==", "", 1);
    printf("> rl_line_buffer = '%s'\n", rl_line_buffer);

    press_down();
    press_down();
    press_down();
    press_down();
    press_down();
    press_down();
    press_down();
    test_assert_str(rl_line_buffer, "==", "", 1);
    printf("> rl_line_buffer = '%s'\n", rl_line_buffer);

    printf("\nPressing up 5000 times... "
            "This should display the last/oldest item in the history.\n");
    for (int i = 0; i < 5000; ++i) {
        key_up(1, 65);
    }
    test_assert_str(rl_line_buffer, "==", "echo Command 066 > /dev/null", 28);
    printf("> rl_line_buffer = '%s'\n", rl_line_buffer);
});


subtest("Incremental search",
{
    read_command();
    press_up();
    press_up();
    press_up();
    test_assert_str(rl_line_buffer, "==", "echo Command 634 > /dev/null", 28);
    printf("> rl_line_buffer = '%s'\n\n", rl_line_buffer);
   
    read_command();
    press_up();
    test_assert_str(rl_line_buffer, "==", "sleep 1 # yawn", 14);
    printf("> rl_line_buffer = '%s'\n\n", rl_line_buffer);

    press_up();
    test_assert_str(rl_line_buffer, "==", "sleep 26", 8);
    printf("> rl_line_buffer = '%s'\n\n", rl_line_buffer);

    press_up();
    test_assert_str(rl_line_buffer, "==", "sleep 26", 8);
    printf("> rl_line_buffer = '%s'\n\n", rl_line_buffer);

    press_down();
    test_assert_str(rl_line_buffer, "==", "sleep 1 # yawn", 14);
    printf("> rl_line_buffer = '%s'\n\n", rl_line_buffer);
    press_down();
    press_down();
    press_down();

    read_command();
    press_up();
    test_assert_str(rl_line_buffer, "==", "echo Command 066 > /dev/null", 28);
    printf("> rl_line_buffer = '%s'\n\n", rl_line_buffer);

    read_command();
    press_up();
    char *cmd = "echo Hello world this is a nice long message to save to a file > /tmp/testfile1";
    test_assert_str(rl_line_buffer, "==", cmd, strlen(cmd));
    printf("> rl_line_buffer = '%s'\n\n", rl_line_buffer);

    press_up();
    press_up();
    press_down();
    press_down();
    press_down();
    test_assert_str(rl_line_buffer, "==", "", 1);
    printf("> rl_line_buffer = '%s'\n\n", rl_line_buffer);

    read_command();
    press_up();
    test_assert_str(rl_line_buffer, "==", "z", 1);
    printf("> rl_line_buffer = '%s'\n\n", rl_line_buffer);

    read_command();
    press_up();
    press_up();
    press_up();
    press_up();
    press_up();

    cmd = "cat /etc/passwd | sort | grep s | sed s/:.*// | tail -n 3 | head -n 2";
    test_assert_str(rl_line_buffer, "==", cmd, strlen(cmd));
    printf("> rl_line_buffer = '%s'\n\n", rl_line_buffer);

    read_command();
    press_up();
    cmd = "make me a sandwich";
    test_assert_str(rl_line_buffer, "==", "make me a sandwich", strlen(cmd));
    printf("> rl_line_buffer = '%s'\n\n", rl_line_buffer);
});

test_end
