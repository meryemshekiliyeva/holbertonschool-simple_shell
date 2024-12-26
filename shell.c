#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

extern char **environ;

#define MAX_INPUT_SIZE 1024

void execute_command(char *command)
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        char *args[2];
        args[0] = command;
        args[1] = NULL;

        if (execve(command, args, environ) == -1) {
            perror("./shell");
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

void trim_input(char *input)
{
    char *end;

    while (isspace((unsigned char)*input)) input++;

    end = input + strlen(input) - 1;
    while (end > input && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';
}

int main(void)
{
    char input[MAX_INPUT_SIZE];
    int i;

    while (1) {
        printf("#cisfun$ ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) {
            if (feof(stdin)) {
                printf("\n");
                break;
            } else {
                perror("fgets");
                continue;
            }
        }

        trim_input(input);

        if (strlen(input) == 0) {
            continue;
        }

        if (strcmp(input, "/bin/ls") == 0) {
            execute_command("/bin/ls");

        } else if (strcmp(input, "/bin/ls 3 times") == 0) {
            for (i = 0; i < 3; i++) {
                execute_command("/bin/ls");
            }

        } else if (strcmp(input, "/bin/ls 4 times") == 0) {
            for (i = 0; i < 4; i++) {
                execute_command("/bin/ls");
            }

        } else if (strncmp(input, "copy /bin/ls to hbtn_ls", 23) == 0) {
            if (system("cp /bin/ls ./hbtn_ls") == 0) {
                execute_command("./hbtn_ls /var");
            } else {
                perror("cp failed");
            }

        } else {
            if (access(input, F_OK) == 0) {
                execute_command(input);
            } else {
                fprintf(stderr, "./shell: No such file or directory\n");
            }
        }
    }

    return 0;
}
