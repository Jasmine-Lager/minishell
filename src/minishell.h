/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 18:27:58 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/11 13:59:15 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>            // INT_MAX, INT_MAX
# include <readline/history.h>  // add_history, rl_clear_history
# include <readline/readline.h> // readline, rl_on_new_line, rl_replace_line,
# include <stdio.h>             // printf, perror
								// rl_redisplay
# include <dirent.h>            // opendir, readdir, closedir
# include <signal.h>            // signal, sigaction, sigemptyset, sigaddset
# include <stdbool.h>           // bool flags
# include <stdlib.h>            // malloc, free, exit, getenv, EXIT_SUCCESS
# include <string.h>            // strerror
# include <sys/ioctl.h>         // ioctl
# include <sys/stat.h>          // stat, lstat, fstat
# include <sys/types.h>         // general types, used with wait/fork/etc.
# include <sys/wait.h>          // wait, waitpid, wait3, wait4
# include <term.h>              // tgetent, tgetflag, tgetnum, tgetstr, tgoto,
								// tputs
# include <termios.h>           // tcsetattr, tcgetattr
# include <unistd.h>            // write, access, read, close, fork, execve,
								// dup,dup2,chdir, unlink, getcwd, kill, isatty,
								// ttyname, ttyslot
# include "libft.h"
# include <errno.h>
# include <fcntl.h> //open

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
	t_quotes		dominant_type;
	int				mixed_quotes;
	int				needs_processing;
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

typedef struct s_expand
{
	char			*result;
	t_token			*current;
	t_token			*last;
	int				nbr_split;
	int				i;
	int				*i_start_split;
	int				*i_end_split;
	bool			can_be_rm;
	int				dquote;
	int				squote;
	int				len;
	int				check;
	int				fd;
}					t_expand;

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
	int nbr_heredoc;
	int exit_code;
	int needs_continuation;
	struct termios orig_term;
	int term_saved;
}					t_mini;

// builtins_more.c
int					builtin_export(t_mini *var, char **argv);
int					builtin_unset(t_mini *var, char **argv);

// builtins_print_export.c
void				print_export(t_mini *var);

// builtins_utils.c
int					export_message(char *arg);
bool				is_n_flag(char *str);
int					is_valid_number(char *str);
int					is_builtin(char *cmd);
int					execute_builtin(t_mini *var, char **argv);

// builtins.c
int					builtin_echo(char **argv);
int					builtin_cd(t_mini *var, char **argv);
int					builtin_pwd(void);
int					builtin_env(t_mini *var, char **argv);
int					builtin_exit(t_mini *var, char **argv);

// clean_up.c
void				free_tokens(t_mini *var);
void				free_var_exit(t_mini *var, int exit_code);
void				free_one_line(t_mini *var);
void				wait_for_children(t_mini *var, pid_t last_child_pid, int i,
						int status);

// commands_utils.c
char				*read_continuation(t_mini *var, char *current_line);

// commands.c
int					handle_line_continuation(t_mini *var);
void				handle_command(t_mini *var);

// environment.c
char				*find_env_var(char **envp, char *key);

// environment_utils.c
int					count_env_vars(char **envp);
int					find_env_index(char **envp, char *key);
int					update_env_var(t_mini *var, char *key, char *value);
int					set_env_var(t_mini *var, char *key, char *value);
int					unset_env_var(t_mini *var, char *key);

// errors.c
void				error_exit(t_mini *var, char *str);
void				error_exit_code(t_mini *var, char *str, int code);
void				access_error(t_mini *var, char **path, char *str, int code);
void				dup2_error(t_mini *var);

// execute_prepare.c
int					already_has_path(t_mini *var, char **path, char *cmd);
void				find_path_to_cmd(t_mini *var, char **path, char *cmd,
						int j);
bool				cpy_content_to_argv(char **dst_argv, t_token *ptr,
						size_t argv_len);
t_token				*find_start_of_nth_cmd(t_mini *var, int cmd_n);
void				prepare_argv_and_redir(t_mini *var, int cmd_n);

// execute.c
bool				execute_cmds(t_mini *var);

// expand_len.c
int					len_keyword(char *str);
int					my_isspace(char c);
void				count_split(char *str, t_expand *exp);
int					count_env_var_len(t_mini *var, char *str, int *i,
						t_expand *exp);
int					len_expanded(t_mini *var, char *str, t_expand *exp);

// expand_utils.c
int					mark_where_to_split(t_expand *exp, char *result, int j);
char				*cpy_expanded(t_mini *var, char *str, char *result,
						t_expand *exp);
int					cpy_env_var(t_mini *var, char *str, int *i, char *dst);
void				empty_token(t_mini *var, t_token *last, t_token **current,
						char *expanded);
t_expand			*init_expanded(t_token *first_token);

// expand.c
void				expand_str(t_mini *var, char *str, t_expand *exp);
int					add_new_token(t_mini *var, t_expand *exp, int from, int to);
int					split_words(t_mini *var, t_expand *exp);
int					save_result_in_token(t_mini *var, t_expand *exp);
bool				expand_tokens(t_mini *var);

// heredoc_expand.c
int					cpy_env_var_delim(t_mini *var, char *str, int *i,
						char *dst);
char				*cpy_expanded_delim(char *str, char *result);
int					len_expanded_heredoc(t_mini *var, char *str);
char				*cpy_expanded_heredoc(t_mini *var, char *str, char *result);
char				*expand_heredoc(t_mini *var, char *str, t_expand *exp);

// heredoc.c
void				read_heredoc(t_mini *var, char *delim, t_expand *exp,
						bool delim_quoted);
void				heredoc_to_file(t_mini *var, char *filename, char **delim,
						t_expand *exp);
bool				heredoc(t_mini *var, t_token *delim, t_expand *exp);

// initialize_minishell.c
void				initialize_minishell(t_mini *var, int argc, char **argv,
						char **envp);

// main.c
int					main(int argv, char **argc, char **envp);

// parse.c
t_quotes			analyze_token_quotes(char *content);
void				set_token_quote_info(t_token *token);
int					create_first_token(t_mini *var, int *start_token,
						int *end_token);
int					append_token(t_mini *var, int *start_token, int *end_token,
						t_token **last);
bool				parse(t_mini *var);

// pipes.c
bool				create_pipes(t_mini *var);
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
void				redirect_in_out_to_pipes(t_mini *var, int cmd_n);
bool				open_redir_infile(char *infile, bool heredoc);
bool				open_redir_outfile(char *outfile, bool append);
void				process_cmd(t_token *ptr, t_token **cmd, int *argv_len);
int					redir_files_and_count_argv_len(t_token *ptr, t_token **cmd,
						int argv_len);

// rm_quotes_delim.c
int					len_no_quotes(char *delim, bool *quoted, int dqote,
						int squote);
char				*cpy_no_quotes(char *str, char *result);
bool				rm_quotes_delim(t_mini *var, char **delim, t_expand *exp);

// signals_handling.c
void				handle_ctrl_c(int signal_number);
void				handle_ctrl_c_execution(int signal_number);
void				handle_ctrl_c_heredoc(int signal_number);

// signals.c
void				signals_setup(void);
void				signals_execution(void);
void				signals_heredoc(void);
void				signals_child(void);

// tmp_file.c
char				*get_tmp_file_name(t_mini *var);
int					open_tmp_file(t_mini *var, char *filename, t_expand *exp);

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
void				check_cmd_flag_word(t_token *new, t_token *last);
void				find_token_type(t_mini *var, t_token *new, t_token *last);

// validate_syntax_metachars.c
bool				has_trailing_pipe(t_mini *var);
bool				validate_syntax(t_mini *var);

#endif
