#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <limits.h>            // INT_MAX, INT_MAX
# include <readline/readline.h> // readline, rl_on_new_line, rl_replace_line,
// rl_redisplay
# include <dirent.h>           // opendir, readdir, closedir
# include <readline/history.h> // add_history, rl_clear_history
# include <signal.h>           // signal, sigaction, sigemptyset, sigaddset
# include <stdbool.h>          // bool flags
# include <stdio.h>            // printf, perror
# include <stdlib.h>           // malloc, free, exit, getenv
# include <string.h>           // strerror
# include <sys/ioctl.h>        // ioctl
# include <sys/stat.h>         // stat, lstat, fstat
# include <sys/types.h>        // general types, used with wait/fork/etc.
# include <sys/wait.h>         // wait, waitpid, wait3, wait4
# include <term.h>             // tgetent, tgetflag, tgetnum, tgetstr, tgoto,
// tputs
// or use <curses.h> if <term.h> isn't available
# include <termios.h> // tcsetattr, tcgetattr
# include <unistd.h>  // write, access, open, read, close, fork,
// execve, getcwd, chdir, unlink, dup, dup2, isatty, ttyname, ttyslot, kill

extern volatile sig_atomic_t g_signal; // only global allowed

typedef struct s_token
{
	char			*content;
	int				type;
	struct s_token	*next;
}					t_token;

typedef struct s_prompt
{
	char			**args;
	char			*input_file;
	char			*output_file;
	int				append_mode;
	int				builtin_type;
}					t_prompt;
