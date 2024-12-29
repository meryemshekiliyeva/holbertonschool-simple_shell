# Simple Shell Project  
![Holberton School Logo](https://i.pinimg.com/originals/c5/a6/d4/c5a6d4a0d33151ac29bfe66d2cfd4ab1.png)  

## Description  
The Simple Shell project is a basic implementation of a Unix shell. It provides users with the ability to execute commands, navigate the file system, and run programs in both interactive and non-interactive modes.  

## Features  
- **Interactive Mode:** Displays a prompt, waits for user input, and executes commands.  
- **Non-Interactive Mode:** Executes commands from a script or file without user interaction.  
- **Error Handling:** Proper error messages for unsupported commands or incorrect syntax.  
- **Environment Management:** Handles environment variables and pathways.  
- **Command Execution:**  
  - Displays a prompt and waits for user input.  
  - Executes commands entered by the user, ending with a newline (when the *ENTER* key is pressed).  
  - Re-displays the prompt after each executed command.  
- **Exit Behavior:**  
  - Entering `exit` will terminate the shell and return a status of 0.  
  - Entering `exit [status]` will terminate the shell and return the specified exit status (0–255).  
  - Using *Ctrl+D* will terminate the shell (EOF).  
- **Command Line Arguments:** Supports commands with arguments and full file paths.  

## Requirements  
- **System:** Ubuntu 20.04 LTS  
- **Compiler:** GCC with the following options:  
  ```bash  
  -Wall -Werror -Wextra -pedantic -std=gnu89  
Code Style: Betty style guidelines
Authorized Functions and Macros
The project utilizes the following standard library functions and system calls:

access (man 2 access)
chdir (man 2 chdir)
close (man 2 close)
closedir (man 3 closedir)
execve (man 2 execve)
exit (man 3 exit)
_exit (man 2 _exit)
fflush (man 3 fflush)
fork (man 2 fork)
free (man 3 free)
getcwd (man 3 getcwd)
getline (man 3 getline)
isatty (man 3 isatty)
kill (man 2 kill)
malloc (man 3 malloc)
open (man 2 open)
opendir (man 3 opendir)
perror (man 3 perror)
read (man 2 read)
readdir (man 3 readdir)
signal (man 2 signal)
stat (man 2 stat)
lstat (man 2 lstat)
fstat (man 2 fstat)
strtok (man 3 strtok)
wait (man 2 wait)
waitpid (man 2 waitpid)
wait3 (man 2 wait3)
wait4 (man 2 wait4)
write (man 2 write)
Compilation
To compile the shell, use the following command:

bash
Copy code
gcc -Wall -Werror -Wextra -pedantic *.c -o hsh  
