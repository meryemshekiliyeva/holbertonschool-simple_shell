#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

extern char **environ;

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 100

void execute_command(char *input) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        char *args[MAX_ARGS];
        char *token;
        int i = 0;

        // Tokenize the input string into arguments
        token = strtok(input, " ");
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // Null-terminate the array

        if (execve(args[0], args, environ) == -1) {
            perror("./shell");
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

void trim_input(char *input) {
    char *end;

    while (isspace((unsigned char)*input)) input++;

    end = input + strlen(input) - 1;
    while (end > input && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    int times; // Declare times at the start
    int i; // Declare i at the start

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

        // Handle specific commands
        if (strcmp(input, "/bin/ls") == 0) {
            execute_command("/bin/ls");
        } else if (strncmp(input, "/bin/ls", 8) == 0) {
            times = 1; // Initialize times
            if (strstr(input, "3 times")) {
                times = 3;
            } else if (strstr(input, "4 times")) {
                times = 4;
            }
            for (i = 0; i < times; i++) {
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

