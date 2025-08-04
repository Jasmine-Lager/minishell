/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:30:51 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/04 21:01:43 by ksevciko         ###   ########.fr       */
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
# include <unistd.h>  // write, access, read, close, fork,
// execve, getcwd, chdir, unlink, dup, dup2, isatty, ttyname, ttyslot, kill
# include <fcntl.h> //open
# include "libft.h"

extern volatile sig_atomic_t	g_signal; // only global allowed

typedef enum e_token_type {
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC
}	t_token_type;

typedef enum e_quotes {
	NONE,
	SINGLE,
	DOUBLE
}	t_quotes;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
	t_quotes		quotes;
	struct s_token	*next;
}					t_token;

// typedef struct s_command
// {
// 	char			**args;
// 	char			*input_file;
// 	char			*output_file;
// 	int				append_mode;
// 	int				builtin_type;
// }					t_command;

typedef struct s_mini //stores all variables usefull for the whole program
{
	char	**envp;
	char	**paths;
	char	*line;
	t_token	*tokens;
	char	*infile;
	char	*outfile;
	bool	append_mode;
	bool	here_doc;
	int		nbr_pipes;
	int		(*pipes)[2];
	char	*cmd;
	char	**argv_for_cmd;
	int		exit_code; //should only be used for pipes, not for signals (is here for expanding $?)
}	t_mini;

//main.c
int		main(int argv, char **argc, char **envp);

//initialize_var.c
char	*find_env_var(char **envp, char *key);
void	initialize_var(t_mini *var, int argc, char **argv, char **envp);

//pipes.c
void	create_pipes(t_mini *var);
void	close_pipes(t_mini *var);
void	in_out_for_1st_cmd(t_mini *var);
void    in_out_for_last_cmd(t_mini *var);
void	redirect_in_out_fds(t_mini *var, int cmd_n);

//clean_up.c
void	free_var_exit(t_mini *var, int exit_code);
void	other_error(t_mini *var, char *str);
void	command_not_found(t_mini *var, char **path);
void	dup2_error(t_mini *var);

#endif