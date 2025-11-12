# 🐚 Minishell
*A simple Unix shell implementation for 42 School*

**Authors:** Jasmine Lager (jlager) & Kaťa Sevčíková (ksevciko)  
**Location:** 42 Prague  
**Project:** Minishell - As beautiful as a shell  

---

## 📚 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Architecture](#architecture)
- [Installation](#installation)
- [Usage](#usage)
- [Implementation Details](#implementation-details)
- [Built-in Commands](#built-in-commands)
- [Testing](#testing)
- [Resources](#resources)

---

## 🎯 Overview

Minishell is a 42 School project that implements a simplified Unix shell, providing a basic command-line interface similar to bash. The project demonstrates deep understanding of Unix system calls, process management, and signal handling while adhering to strict 42 Norm coding standards.

The shell provides essential functionality including command parsing, execution, pipes, redirections, environment variable handling, and built-in commands, all while maintaining proper memory management and handling edge cases gracefully.

---

## ✨ Features

### Core Functionality
- **Interactive shell prompt** with command history (using readline)
- **Command execution** using PATH environment variable
- **Pipes** (`|`) for command chaining
- **Redirections**:
  - Input redirection (`<`)
  - Output redirection (`>`)
  - Append mode (`>>`)
  - Heredoc (`<<`)
- **Environment variables** expansion (`$VAR`)
- **Exit status** handling (`$?`)
- **Quote handling**:
  - Single quotes (`'`) - prevent all expansions
  - Double quotes (`"`) - prevent expansions except for `$`
- **Signal handling**:
  - `Ctrl-C` - interrupt current command
  - `Ctrl-D` - exit shell
  - `Ctrl-\` - quit signal (ignored in interactive mode)

### Built-in Commands
- `echo` (with -n option)
- `cd` (with relative/absolute paths)
- `pwd` (print working directory)
- `export` (set environment variables)
- `unset` (remove environment variables)
- `env` (display environment)
- `exit` (exit shell with status)

---

## 📁 Project Structure

```
minishell/
│
├── src/                        # Source files
│   ├── main.c                  # Entry point and main loop
│   ├── initialize_minishell.c  # Initialization routines
│   │
│   ├── commands.c              # Command handling
│   ├── commands_utils.c        # Command utilities
│   │
│   ├── parse.c                 # Input parsing
│   ├── token_define.c          # Token definition
│   ├── token_type.c            # Token type assignment
│   │
│   ├── execute.c               # Command execution
│   ├── execute_prepare.c       # Execution preparation
│   ├── pipes.c                 # Pipe management
│   ├── redirect.c              # Redirection handling
│   │
│   ├── expand.c                # Variable expansion
│   ├── expand_len.c            # Expansion length calculation
│   ├── expand_utils.c          # Expansion utilities
│   │
│   ├── heredoc.c               # Heredoc implementation
│   ├── heredoc_expand.c        # Heredoc expansion
│   ├── tmp_file.c              # Temporary file handling
│   │
│   ├── builtins.c              # Core built-ins (echo, cd, pwd, env, exit)
│   ├── builtins_more.c         # Additional built-ins (export, unset)
│   ├── builtins_print_export.c # Export printing
│   ├── builtins_utils.c        # Built-in utilities
│   │
│   ├── environment.c           # Environment variable handling
│   ├── environment_utils.c     # Environment utilities
│   │
│   ├── quotes_handling.c       # Quote processing
│   ├── quotes_handling_utils.c # Quote utilities
│   ├── rm_quotes_delim.c       # Quote removal
│   │
│   ├── signals.c               # Signal setup
│   ├── signals_handling.c      # Signal handlers
│   │
│   ├── validate_syntax_metachars.c # Syntax validation
│   ├── errors.c                # Error handling
│   └── clean_up.c              # Memory cleanup
│
├── libft/                      # Custom C library
├── minishell.h                 # Main header file
├── Makefile                    # Build configuration
├── en_subject.pdf              # Project requirements
└── the_norm.pdf                # 42 coding standards
```

---

## 🏗️ Architecture

### Data Structures

#### Main Structure (`t_mini`)
The core structure that maintains the shell state:
```c
typedef struct s_mini {
    char    **envp;              // Environment variables
    char    **paths;             // PATH directories
    char    *line;               // Current input line
    t_token *tokens;             // Parsed token list
    int     nbr_pipes;           // Number of pipes
    int     (*pipes)[2];         // Pipe file descriptors
    char    *cmd;                // Current command
    char    **argv_for_cmd;      // Command arguments
    int     nbr_heredoc;         // Heredoc count
    int     exit_code;           // Last exit code
    int     needs_continuation;  // Line continuation flag
    struct termios orig_term;    // Terminal settings
    int     term_saved;          // Terminal save status
}   t_mini;
```

#### Token Structure (`t_token`)
Linked list for parsed input:
```c
typedef struct s_token {
    char            *content;    // Token content
    t_token_type    type;        // Token type
    t_quotes        quotes;      // Quote type
    struct s_token  *next;       // Next token
}   t_token;
```

### Program Flow

1. **Initialization** → Set up environment, terminal settings
2. **Main Loop:**
   - Read input using `readline()`
   - Parse input into tokens
   - Validate syntax
   - Expand variables
   - Execute commands/built-ins
   - Clean up resources
3. **Exit** → Free memory, restore terminal

---

## 🔧 Installation

### Prerequisites
- GCC compiler
- Make
- readline library
- Linux/macOS system

### Compilation

```bash
# Clone the repository (if using git)
git clone <repository-url>
cd minishell

# Compile the project
make

# Clean object files
make clean

# Remove all generated files
make fclean

# Recompile from scratch
make re
```

The Makefile automatically handles:
- Platform detection (Linux/macOS)
- libft compilation
- readline library linking

---

## 💻 Usage

### Starting the Shell
```bash
./minishell
```

### Examples

#### Basic Commands
```bash
$ ls -la
$ echo "Hello, World!"
$ cat file.txt | grep pattern | wc -l
```

#### Redirections
```bash
$ echo "content" > output.txt
$ cat < input.txt
$ echo "append" >> file.txt
$ cat << EOF
> multiline
> input
> EOF
```

#### Environment Variables
```bash
$ export MY_VAR=value
$ echo $MY_VAR
$ unset MY_VAR
$ env
```

#### Built-in Commands
```bash
$ cd /path/to/directory
$ pwd
$ exit 42
```

---

## ⚙️ Implementation Details

### Tokenization Process
1. **Lexical Analysis**: Break input into tokens based on spaces and metacharacters
2. **Quote Processing**: Handle single/double quotes for grouping
3. **Type Assignment**: Classify tokens (command, argument, pipe, redirection)
4. **Syntax Validation**: Check for syntax errors

### Variable Expansion
- **Environment variables**: `$VAR` → value
- **Exit status**: `$?` → last command exit code
- **Quote-aware**: Expansion respects quote contexts
- **Word splitting**: Expanded values are split on whitespace

### Signal Handling
- **Interactive mode**: Custom handlers for user interaction
- **Execution mode**: Different handlers during command execution
- **Heredoc mode**: Special handling for heredoc input
- **Child processes**: Reset to default signal behavior

### Memory Management
- Systematic cleanup after each command
- Proper error path cleanup
- No memory leaks (validated with valgrind)
- Follows 42 Norm requirements

---

## 🧪 Testing

### Manual Testing
Compare behavior with bash:
```bash
# Test pipes
echo "test" | cat | cat | cat

# Test redirections
< input.txt cat | grep pattern > output.txt

# Test quotes
echo '$HOME' vs echo "$HOME"

# Test built-ins
export TEST=123 && echo $TEST && unset TEST
```

### Memory Testing
```bash
# Run with valgrind
valgrind --leak-check=full --show-leak-kinds=all ./minishell

# With readline suppression
valgrind --suppressions=readline.supp --leak-check=full ./minishell
```

### Edge Cases
- Empty input
- Only spaces
- Unclosed quotes
- Invalid syntax (e.g., `|`, `>`, `>>` at start/end)
- Command not found
- Permission denied
- Signal handling during different states

---

## 📚 Resources

- **Project Documentation**:
  - `en_subject.pdf` - Official project requirements
  - `the_norm.pdf` - 42 coding standards

- **System Programming**:
  - [The Linux Programming Interface](https://man7.org/tlpi/)
  - [Advanced Programming in the UNIX Environment](https://www.apuebook.com/)

- **Shell Implementation**:
  - [Bash Reference Manual](https://www.gnu.org/software/bash/manual/)
  - [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)

---

## 📝 Notes

- The shell implements mandatory features as per 42 requirements
- We have not found absolutely no leeks, even throe thorough testing with our local tutors :)
- Follows strict 42 Norm: functions ≤ 25 lines, specific formatting
- readline library handles command history and line editing
- Project tested on Linux and macOS environments

---

## 🏆 Acknowledgments

Special thanks to the 42 Prague community for support and testing!

---

*Made with ❤️ at 42 Prague*
