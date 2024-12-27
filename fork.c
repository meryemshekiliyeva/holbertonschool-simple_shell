#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "main.h"

#define MAX_INPUT 1024
#define DELIM " \t\r\n\a"

char *find_command(char *command);

int main(void)
{
    char *input = NULL;
    size_t len = 0;
    char *argv[100];
    pid_t pid;
    int status;

    while (1)
    {
        printf(":) ");
        if (getline(&input, &len, stdin) == -1)
        {
            perror("getline");
            free(input);
            exit(EXIT_FAILURE);
        }

        /* Tokenize the input */
        char *token;
        int i = 0;

        token = strtok(input, DELIM);
        while (token != NULL)
        {
            argv[i++] = token;
            token = strtok(NULL, DELIM);
        }
        argv[i] = NULL;

        if (argv[0] == NULL)
        {
            continue; /* Empty command */
        }

        /* Check for exit command */
        if (strcmp(argv[0], "exit") == 0)
        {
            free(input);
            exit(0);
        }

        /* Locate the command */
        char *command_path;
        command_path = find_command(argv[0]);
        if (command_path == NULL)
        {
            printf("%s: command not found\n", argv[0]);
            continue;
        }

        /* Fork and execute */
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(input);
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            /* Child process */
            execve(command_path, argv, environ);
            perror("execve"); /* Only reached if execve fails */
            free(input);
            exit(EXIT_FAILURE);
        }
        else
        {
            /* Parent process */
            wait(&status);
        }

        free(command_path);
    }

    free(input);
    return 0;
}

char *find_command(char *command)
{
    char *path;
    char *path_copy;
    char *dir;
    char *full_path;

    path = getenv("PATH");
    if (!path)
        return NULL;

    path_copy = strdup(path);
    full_path = malloc(MAX_INPUT);

    dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
        snprintf(full_path, MAX_INPUT, "%s/%s", dir, command);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path; /* Command found */
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    free(full_path);
    return NULL; /* Command not found */
}

