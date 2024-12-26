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

void print_env(void);
char *find_command_in_path(char *command);
void execute_command(char *input);
void trim_input(char *input); // Function prototype

// Function to trim newline character from input
void trim_input(char *input) {
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0'; // Replace newline with null terminator
    }
}

char *find_command_in_path(char *command) {
    char *path = getenv("PATH");
    char *token = strtok(path, ":");
    char full_path[MAX_INPUT_SIZE];

    while (token != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", token, command);
        if (access(full_path, X_OK) == 0) {
            return strdup(full_path); // Return the full path if found
        }
        token = strtok(NULL, ":");
    }
    return NULL; // Command not found
}

void execute_command(char *input) {
    char *args[MAX_ARGS];
    char *token;
    pid_t pid;
    int status;
    int i = 0;
    char *command_path; // Declare here

    // Tokenize the input string into arguments
    token = strtok(input, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL; // Null-terminate the array

    // Check if the command exists in PATH
    command_path = find_command_in_path(args[0]); // Now it's declared before usage
    if (command_path != NULL) {
        // Command exists, fork and execute
        pid = fork();
        if (pid == -1) {
            perror("fork");
            free(command_path);
            return;
        }

        if (pid == 0) {
            execve(command_path, args, environ);
            perror("execve"); // If execve fails
            exit(EXIT_FAILURE);
        } else {
            waitpid(pid, &status, 0);
        }
        free(command_path); // Free the allocated memory
    } else {
        // Command not found
        fprintf(stderr, "./hsh: %s: not found\n", args[0]);
    }
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

        // Trim input and handle empty input
        trim_input(input);
        if (strlen(input) == 0) {
            continue;
        }

        // Execute the command
        execute_command(input);
    }

    return 0;
}

