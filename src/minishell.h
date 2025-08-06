/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:30:51 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/06 23:17:54 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>            // INT_MAX, INT_MAX
# include <readline/readline.h> // readline, rl_on_new_line, rl_replace_line,
// rl_redisplay
# include <dirent.h>           // opendir, readdir, closedir
# include <readline/history.h> // add_history, rl_clear_history
# include <signal.h>           // signal, sigaction, sigemptyset, sigaddset
# include <stdbool.h>          // bool flags
# include <stdio.h>            // printf, perror
# include <stdlib.h>           // malloc, free, exit, getenv, EXIT_SUCCESS
# include <string.h>           // strerror
# include <sys/ioctl.h>        // ioctl
# include <sys/stat.h>         // stat, lstat, fstat
# include <sys/types.h>        // general types, used with wait/fork/etc.
# include <sys/wait.h>         // wait, waitpid, wait3, wait4
# include <term.h>             // tgetent, tgetflag, tgetnum, tgetstr, tgoto,
// tputs
// or use <curses.h> if <term.h> isn't available
# include <termios.h> // tcsetattr, tcgetattr
# include <unistd.h>  // write, access, read, close, fork,
// execve, getcwd, chdir, unlink, dup, dup2, isatty, ttyname, ttyslot, kill
# include "libft.h"
# include <fcntl.h> //open

// ----------ASCI escape codes for text formating----------
# define RESET "\x1b[0m"

// Text styles
# define BOLD "\x1b[1m"
# define ITALIC "\x1b[3m"
# define UNDERLINE "\x1b[4m"
# define DIM "\x1b[2m"

// Text (foreground) color
// Text colors are \x1b[3Xm (normal) or \x1b[9Xm (bright).
# define RED "\x1b[91m"
# define PURPLE "\x1b[35m"
# define BLUE "\x1b[34m"
# define CYAN "\x1b[94m"
# define GREEN "\x1b[92m"
# define GOLD "\x1b[33m"
# define WHITE "\x1b[37m"
# define GRAY "\x1b[90m"
# define BLACK "\x1b[30m"

// Background colors
// Background colors are \x1b[4Xm (normal) or \x1b[10Xm (bright).
# define BG_RED "\x1b[101m"
# define BG_PURPLE "\x1b[45m"
# define BG_BLUE "\x1b[44m"
# define BG_CYAN "\x1b[106m"
# define BG_GREEN "\x1b[102m"
# define BG_GOLD "\x1b[43m"
# define BG_WHITE "\x1b[47m"
# define BG_GRAY "\x1b[100m"
# define BG_BLACK "\x1b[40m"
// Exaple use:
//	ft_printf(UNDERLINE GREEN BG_BLACK "Success: " RESET "Operation complete.");
//	write(1, BOLD CYAN "Load..." RESET, ft_strlen(BOLD CYAN "Load..." RESET));

extern volatile sig_atomic_t	g_signal; // only global allowed

typedef enum e_token_type
{
	WORD,
	CMD,//not sure this will ever be used
	FLAG,//or this
	PIPE,
	REDIR_IN,
	INFILE,
	REDIR_OUT,
	REDIR_APPEND,
	OUTFILE,
	HEREDOC,
	DELIMITER
}					t_token_type;

typedef enum e_quotes
{
	NONE,
	SINGLE,
	DOUBLE
}					t_quotes;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
	t_quotes		quotes;
	struct s_token	*next;
}					t_token;

typedef struct s_mini // stores all variables usefull for the whole program
{
	char	**envp;
	char	**paths;
	char	*line;
	t_token	*tokens;
	char	*infile;
	char	*outfile;
	bool	append_mode;
	bool	here_doc;
	char	*delimiter;
	int		nbr_pipes;
	int		(*pipes)[2];
	char	*cmd;
	char	**argv_for_cmd;
	int		exit_code; // should only be used for pipes,
		// not for signals (is here for expanding $?)
}					t_mini;

// main.c
int					main(int argv, char **argc, char **envp); //why so big indentation? i think it is more readable if it is as small as possible, so more functions fit on single line 

// initialize_var.c
char				*find_env_var(char **envp, char *key);
void				initialize_minishell(t_mini *var, int argc, char **argv,
						char **envp);

//signals.c
void	setup_signals(void);

// parsing.c
void				parse(t_mini *var);

// commands.c
void				handle_command(t_mini *var);

// pipes.c
void				create_pipes(t_mini *var);
void				close_pipes(t_mini *var);

// redirecting.c
void				in_out_for_1st_cmd(t_mini *var);
void				in_out_for_last_cmd(t_mini *var);
void				redirect_for_pipes(t_mini *var, int cmd_n);

// execution.c
void				find_path(t_mini *var, char **path, char *cmd);
void				cmds_to_struct(t_mini *var);
void				wait_for_children(t_mini *var, pid_t last_child_pid);
void				execute_cmds(t_mini *var);

// clean_up.c
void				free_var_exit(t_mini *var, int exit_code);
void				other_error(t_mini *var, char *str);
void				command_not_found(t_mini *var, char **path);
void				dup2_error(t_mini *var);

#endif