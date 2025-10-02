/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:30:51 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/02 20:37:02 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>            // INT_MAX, INT_MAX
# include <readline/history.h>  // add_history, rl_clear_history
# include <readline/readline.h> // readline, rl_on_new_line, rl_replace_line,
# include <stdio.h>             // printf, perror
// rl_redisplay
# include <dirent.h>    // opendir, readdir, closedir
# include <signal.h>    // signal, sigaction, sigemptyset, sigaddset
# include <stdbool.h>   // bool flags
# include <stdlib.h>    // malloc, free, exit, getenv, EXIT_SUCCESS
# include <string.h>    // strerror
# include <sys/ioctl.h> // ioctl
# include <sys/stat.h>  // stat, lstat, fstat
# include <sys/types.h> // general types, used with wait/fork/etc.
# include <sys/wait.h>  // wait, waitpid, wait3, wait4
# include <term.h>      // tgetent, tgetflag, tgetnum, tgetstr, tgoto,
// tputs
// or use <curses.h> if <term.h> isn't available
# include <termios.h> // tcsetattr, tcgetattr
# include <unistd.h>  // write, access, read, close, fork,
// execve, getcwd, chdir, unlink, dup, dup2, isatty, ttyname, ttyslot, kill
# include "libft.h"
# include <fcntl.h> //open
# include <errno.h>

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

extern volatile sig_atomic_t g_signal; // only global allowed

typedef enum e_quotes
{
	NONE,
	SINGLE,
	DOUBLE
}					t_quotes;

// Determine the quote structure of a token - more sophisticated analysis
typedef struct s_quote_info
{
	t_quotes dominant_type;	// Overall quote behavior for expansion
	int mixed_quotes;		// Flag indicating complex quote mixing
	int needs_processing;	// Flag for expansion stage
}					t_quote_info;

typedef struct s_quote_counts
{
	int				single_sections;
	int				double_sections;
	int				unquoted_sections;
}					t_quote_counts;

typedef enum e_token_type
{
	WORD,
	CMD,
	FLAG,
	PIPE,
	REDIR_IN,
	INFILE,
	REDIR_OUT,
	REDIR_APPEND,
	OUTFILE,
	HEREDOC,
	DELIMITER
}					t_token_type;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
	t_quotes		quotes;
	struct s_token	*next;
}					t_token;

// typedef struct s_expand //might use this later for word splitting
// {
// 	char	*str;
// 	char	*result;
// 	char	*i;
// 	char	*j;
// 	int		nbr_of_splitting;
// 	int		**i_of_splitting;
// 	bool	can_be_rm;
// }	t_expand;


typedef struct s_mini // stores all variables usefull for the whole program
{
	char **envp;
	char **paths;
	char *line;
	t_token *tokens;
	int nbr_pipes;
	int (*pipes)[2];
	char *cmd;
	char **argv_for_cmd;
	int	nbr_heredoc;
	int exit_code; // should only be used for pipes,
					// not for signals (is here for expanding $?)
}					t_mini;

// clean_up.c
void				free_tokens(t_mini *var);
void				free_var_exit(t_mini *var, int exit_code);
void				free_one_line(t_mini *var);

// command_utils.c

// commands.c
t_token	*find_start_of_nth_cmd(t_mini *var, int cmd_n);
void				handle_command(t_mini *var);

// environment.c
char				*find_env_var(char **envp, char *key);

// errors.c
void				error_exit(t_mini *var, char *str);
void				command_not_found(t_mini *var, char **path);
void				dup2_error(t_mini *var);

// execute.c
void				find_path_to_cmd(t_mini *var, char **path, char *cmd);
void				cpy_content_to_argv(t_mini *var, char **dst_argv, t_token *ptr,
						size_t argv_len);
void				prepare_argv_and_redir(t_mini *var, int cmd_n);
void				wait_for_children(t_mini *var, pid_t last_child_pid);
void				execute_cmds(t_mini *var);

// expand_len.c
int	len_keyword(char *str);
int	count_env_var_len(t_mini *var, char *str, int *i);
int	len_expanded(t_mini *var, char *str);

// expand.c
int cpy_env_var(t_mini *var, char *str, int *i, char *dst);
char	*cpy_expanded(t_mini *var, char *str, char *result, bool delim);
char	*expand_str(t_mini *var, char *str);
void				expand_tokens(t_mini *var);

// heredoc_utils.c
char	*get_tmp_file_name(t_mini *var);
int		open_tmp_file(t_mini *var, char *filename);
int		len_expanded_heredoc(t_mini *var, char *str);
char	*cpy_expanded_heredoc(t_mini *var, char *str, char *result);
char	*expand_heredoc(t_mini *var, char *str);

// heredoc.c
int		len_no_quotes(char *delim, bool *quoted);
bool	rm_quotes_delim(t_mini *var, char **delim);
void	read_heredoc(t_mini *var, char *delim, int fd, bool delim_quoted);
void	heredoc_to_file(t_mini *var, char *filename, char **delim);
void	heredoc(t_mini *var, t_token *delim);

// initialize_minishell.c
void				initialize_minishell(t_mini *var, int argc, char **argv,
						char **envp);

// main.c
int					main(int argv, char **argc, char **envp);

// parse.c
t_quotes			analyze_token_quotes(char *content);
void				set_token_quote_info(t_token *token);
void				create_first_token(t_mini *var, int *start_token,
						int *end_token);
void				append_token(t_mini *var, int *start_token, int *end_token,
						t_token **last);
void				parse(t_mini *var);

// pipes.c
void				create_pipes(t_mini *var);
void				close_pipes(t_mini *var);

// quotes_handling_utils.c
int					is_pure_single(t_quote_counts counts);
int					is_pure_double(t_quote_counts counts);
t_quotes			get_dominant_type(t_quote_counts counts);
void				handle_single_quote(int *in_single, t_quote_counts *counts);
void				handle_double_quote(int *in_double, t_quote_counts *counts);

// quotes_handling.c
t_quote_info		set_quote_behavior(t_quote_counts counts);
t_quote_counts		count_quote_sections(char *content);
t_quote_info		analyze_token_quotes_detailed(char *content);

// redirect.c
void	redirect_in_out_to_pipes(t_mini *var, int cmd_n);
void	open_redir_infile(t_mini *var, char *infile, bool heredoc);
void	open_redir_outfile(t_mini *var, char *outfile, bool append);
void	process_cmd(t_token *ptr, t_token **cmd, int *argv_len);					
int		redir_files_and_count_argv_len(t_mini *var, t_token *ptr,
		t_token **cmd, int argv_len);

// remove_quotes.c
int					calculate_unquoted_length(char *str);
void				copy_without_quotes(char *dst, char *src);
void				remove_quotes_from_token(t_token *token);
void				remove_quotes_from_tokens(t_mini *var);

// signals.c
void				handle_ctrl_c(int signal_number);
void				signals_setup(void);

// token_define.c
int					skip_quoted_section(t_mini *var, int i, char quote);
int					is_token_boundary(t_mini *var, int i);
int					find_complete_token_end(t_mini *var, int start);
int					get_metachar_end(t_mini *var, int start);
void				define_token(t_mini *var, int *start_token, int *end_token,
						t_token *new);

// token_type.c
bool				check_metacharacters(t_mini *var, t_token *new);
bool				check_in_out_delim(t_token *new, t_token *last);
void				find_token_type(t_mini *var, t_token *new, t_token *last);

#endif