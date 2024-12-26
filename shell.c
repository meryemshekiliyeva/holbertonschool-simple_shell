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
    char *args[MAX_ARGS];
    char *token;
    pid_t pid;
    int status;
    int i = 0; // Declare i at the beginning of the function

    // Tokenize the input string into arguments
    token = strtok(input, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL; // Null-terminate the array

    // Check if command exists in PATH
    if (access(args[0], X_OK) == 0) {
        // Command exists, fork and execute
        pid = fork();
        if (pid == -1) {
            perror("fork");
            return;
        }

        if (pid == 0) {
            execve(args[0], args, environ);
            perror("execve"); // If execve fails
            exit(EXIT_FAILURE);
        } else {
            waitpid(pid, &status, 0);
        }
    } else {
        // Command not found
        fprintf(stderr, "Command not found: %s\n", args[0]);
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

        execute_command(input);
    }

    return 0;
}

