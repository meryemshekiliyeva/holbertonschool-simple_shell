üq# Simple Shell Project

## Description
The Simple Shell project is an implementation of a basic Unix shell. This shell allows users to execute commands, navigate the file system, and run programs in both interactive and non-interactive modes.

## Features
- Interactive and non-interactive modes.
- Command execution using `execve`.
- PATH resolution for commands.
- Built-in commands: `exit`, `env`, `cd`, etc.
- Error handling and environment manipulation.

## Requirements
- Ubuntu 20.04 LTS
- GCC compiler with options: `-Wall -Werror -Wextra -pedantic -std=gnu89`
- Betty style checker

## Usage
### Compilation
```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

