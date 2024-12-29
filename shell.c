<<<<<<< HEAD
#include "main.h"
/**
 * execute_command - func for executing command
 * @args: arguments to path
 * @path: path
 */
void execute_command(char **args, char *path)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (execve(path, args, environ) == -1)
		{
			free(path);
			fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (wait(&status) == -1)
		{
			free(path);
			perror("wait failed");
			exit(EXIT_FAILURE);
		}
	}
}
/**
 * parse_command - func for parsing command
 * @u_command: command to be parsed
 * @args: arguments to command
*/
void parse_command(char *u_command, char **args)
{
	char *command = strtok(u_command, " \t");
	int i = 0;

	args[0] = NULL;
	while (command != NULL && i < MAX_LEN - 1)
	{
		args[i] = command;
		i++;
		command = strtok(NULL, " \t");
	}
	args[i] = NULL;
}
/**
 * process_commands - commands processor func
 * @commands: commands
 * @commands_array: array for all commands
*/
void process_commands(char *commands, char **commands_array)
{
	char *command;
	int a = 0;

	command = strtok(commands, "\n");
	while (command != NULL)
	{
		commands_array[a] = command;
		command = strtok(NULL, "\n");
		a++;
	}
	commands_array[a] = NULL;
}
/**
 * handle_commands_array - func for handling array of commands
 * @commands_array: array of commands
 */
void handle_commands_array(char **commands_array)
{
	int a = 0;
	char *command;

	if (strcmp(commands_array[a], "exit") == 0)
		exit(0);
	else if (strcmp(commands_array[a], "env") == 0)
		print_env();
	else
		while (commands_array[a] != NULL)
		{
			command = commands_array[a];
			if (strcmp(command, "exit") == 0 && a > 0)
				exit(2);
			handle_command(command);
			a++;
		}
}
/**
 * main - main func to process all functions
 * Return: integer
 */
int main(void)
{
	char commands[MAX_LEN];
	char *commands_array[MAX_LEN];
	ssize_t read_size;

	while (1)
	{
		read_size = read(STDIN_FILENO, commands, MAX_LEN);
		if (read_size == -1)
		{
			perror("Error reading command");
			exit(EXIT_FAILURE);
		}
		else if (read_size == 0)
			break;
		commands[read_size] = '\0';

		process_commands(commands, commands_array);
		handle_commands_array(commands_array);
	}
	return (0);
=======
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 #include <sys/wait.h>
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <errno.h>
 #include "main.h"
 
 
 /**
  * get_path - returns the PATH
  *
  * Return: the path
  */
 char *get_path(void)
 {
     char **env = environ;
     char *path = NULL;
 
     if (env == NULL)
         return (NULL);
     while (*env != NULL)
     {
         if (strncmp(*env, "PATH=", 5) == 0)
         {
             path = *env + 5;
             return (path);
         }
         env++;
     }
     return (NULL);
 }
 
 
 /**
  * get_full_path - finds the full path of given command
  * @arg: the given argumnet
  * @status: variable for the exit status
  *
  * Return: full path os the given command
  */
 char *get_full_path(char *arg, int *status)
 {
     char *PATH;
     char *dir;
     char *full_path;
 
     if (access(arg, F_OK) == 0)
     {
         full_path = malloc(strlen(arg) + 1);
         strcpy(full_path, arg);
         return (full_path);
     }
     if (get_path() == NULL)
     {
         fprintf(stderr, "./hsh: 1: %s: not found\n", arg);
         *status = 127;
         return (NULL);
     }
     PATH = strdup(get_path());
     dir = strtok(PATH, ":");
     while (dir)
     {
         full_path = malloc(strlen(dir) + strlen(arg) + 2);
         strcpy(full_path, dir);
         strcat(full_path, "/");
         strcat(full_path, arg);
         if (access(full_path, F_OK) == 0)
         {
             free(PATH);
             return (full_path);
         }
         free(full_path);
         dir = strtok(NULL, ":");
     }
     fprintf(stderr, "./hsh: 1: %s: not found\n", arg);
     *status = 127;
     free(PATH);
     return (NULL);
 }
 
 /**
  * set_argv - sets the buffer to the argument vector
  * @buffer: given buffer to set
  * @argv: the ageument vector to set buffers to
  */
 void set_argv(char *buffer, char ***argv)
 {
     size_t argc;
     size_t i;
     char *arg;
     char *copy = strdup(buffer);
 
     arg = strtok(copy, " \n");
     for (argc = 0; arg; argc++)
         arg = strtok(NULL, " \n");
     free(copy);
     *argv = malloc(sizeof(char *) * (argc + 1));
     arg = strtok(buffer, " \n");
     for (i = 0; arg; i++)
     {
         (*argv)[i] = arg;
         arg = strtok(NULL, " \n");
     }
     (*argv)[i] = NULL;
 }
 /**
  * execute - executes the given program
  * @argv: argument vector
  * @status: variable for exit status
  *
  */
 void execute(char **argv, int *status)
 {
     pid_t pid;
     char *command;
 
     if (argv[0] == NULL)
         return;
     command = get_full_path(argv[0], status);
     if (command == NULL)
         return;
     pid = fork();
     if (pid == 0)
         execve(command, argv, environ);
     else
     {
         wait(NULL);
         free(command);
     }
 }
 /**
  * main - the main function
  *
  * Return: exit status
  */
 int main(void)
 {
     char *buffer;
     char **argv;
     size_t size;
     ssize_t read;
     int status = 0;
 
     while (1)
     {
         buffer = NULL;
         size = 0;
         read = getline(&buffer, &size, stdin);
         if (read == -1)
             break;
         set_argv(buffer, &argv);
         execute(argv, &status);
         free(argv);
         free(buffer);
     }
     free(buffer);
     exit(status);
>>>>>>> e6f85ac7d3e0f0b5d11c95b414dd7adfe7416cce
}

