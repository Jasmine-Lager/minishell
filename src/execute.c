/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:11:00 by jasminelage       #+#    #+#             */
/*   Updated: 2025/11/05 14:47:13 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Execute built-in in child process (for pipelines)
static void	execute_builtin_child(t_mini *var)
{
	int	exit_code;

	exit_code = execute_builtin(var, var->argv_for_cmd);
	free_var_exit(var, exit_code);
}

// Execute single built-in command without pipes (in parent)
static bool	execute_single_builtin(t_mini *var, t_token *cmd)
{
	t_token	*ptr;
	int		argv_len;
	int		saved_stdin;
	int		saved_stdout;

	saved_stdin = dup(0);
	saved_stdout = dup(1);
	if (saved_stdin == -1 || saved_stdout == -1)
		return (perror("dup"), 0);
	ptr = var->tokens;
	argv_len = redir_files_and_count_argv_len(var, ptr, &cmd, 1);
	var->argv_for_cmd = malloc((argv_len + 1) * sizeof(char *));
	if (!var->argv_for_cmd)
	{
		close(saved_stdin);
		close(saved_stdout);
		return (write(2, "malloc failed: execute_single_builtin\n", 38), 0);
	}
	cpy_content_to_argv(var, var->argv_for_cmd, cmd, argv_len);
	var->exit_code = execute_builtin(var, var->argv_for_cmd);
	dup2(saved_stdin, 0);
	dup2(saved_stdout, 1);
	close(saved_stdin);
	close(saved_stdout);
	return (1);
}

static pid_t	fork_and_execute_processes(t_mini *var)
{
	int		n;
	pid_t	pid;

	n = -1;
	while (++n <= var->nbr_pipes)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			signals_setup();
			return (-1);
		}
		else if (pid == 0)
		{
			signals_child();
			prepare_argv_and_redir(var, n);
			if (is_builtin(var->argv_for_cmd[0]))
				execute_builtin_child(var);
			execve(var->cmd, var->argv_for_cmd, var->envp);
			perror("execve");
			free_var_exit(var, 1);
		}
	}
	return (pid);
}

bool	execute_cmds(t_mini *var)
{
	pid_t	last_pid;

	if (var->nbr_pipes == 0 && var->tokens && var->tokens->type == CMD
		&& is_builtin(var->tokens->content))
		return (execute_single_builtin(var, NULL));
	if (!create_pipes(var))
		return (0);
	signals_execution();
	last_pid = fork_and_execute_processes(var);
	if (last_pid == -1)
		return (0);
	close_pipes(var);
	wait_for_children(var, last_pid);
	if (var->term_saved)
		tcsetattr(STDIN_FILENO, TCSANOW, &(var->orig_term));
	signals_setup();
	return (1);
}
