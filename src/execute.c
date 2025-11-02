/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:11:00 by jasminelage       #+#    #+#             */
/*   Updated: 2025/11/02 18:20:43 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	already_has_path(t_mini *var, char **path, char *cmd)
{
	struct stat	path_stat;

	*path = ft_strdup(cmd);
	if (!*path)
		error_exit(var, "minishell: malloc failed\n");
	if (is_builtin(cmd))
		return (1);
	if (ft_strchr(cmd, '/'))
	{
		if (access(*path, F_OK) == -1)
			access_error(var, path, ": No such file or directory\n", 127);
		if (stat(*path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
			access_error(var, path, ": Is a directory\n", 126);
		if (access(*path, X_OK) == -1)
		{
			access_error(var, path, ": Permission denied\n", 126);
		}
		return (1);
	}
	return (0);
}

void	find_path_to_cmd(t_mini *var, char **path, char *cmd, int j)
{
	char	*tmp;

	if (!cmd || !*cmd)
		error_exit_code(var, "minishell: invalid command\n", 127);
	if (already_has_path(var, path, cmd))
		return ;
	while (var->paths && var->paths[j])
	{
		free(*path);
		tmp = ft_strjoin(var->paths[j++], "/");
		if (!tmp)
			error_exit(var, "minishell: malloc failed\n");
		*path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!*path)
			error_exit(var, "minishell: malloc failed\n");
		if (access(*path, X_OK) == 0)
			return ;
	}
	access_error(var, path, ": command not found\n", 127);
}

void	cpy_content_to_argv(t_mini *var, char **dst_argv, t_token *ptr,
		size_t argv_len)
{
	size_t	i;

	if (ptr->type != CMD)
		error_exit(var, "missing a command\n");
	dst_argv[0] = ptr->content;
	ptr = ptr->next;
	i = 1;
	while (i < argv_len)
	{
		if (ptr->type == WORD || ptr->type == FLAG)
		{
			dst_argv[i] = ptr->content;
			i++;
		}
		ptr = ptr->next;
	}
	dst_argv[i] = NULL;
}

t_token	*find_start_of_nth_cmd(t_mini *var, int cmd_n)
{
	t_token	*ptr;

	ptr = var->tokens;
	while (ptr && cmd_n > 0)
	{
		if (ptr->type == PIPE)
			cmd_n--;
		ptr = ptr->next;
	}
	if (!cmd_n)
		return (ptr);
	error_exit(var, "missing a command\n");
	return (NULL);
}

void	prepare_argv_and_redir(t_mini *var, int cmd_n)
{
	t_token	*ptr;
	t_token	*cmd;
	int		argv_len;

	redirect_in_out_to_pipes(var, cmd_n);
	ptr = find_start_of_nth_cmd(var, cmd_n);
	cmd = NULL;
	argv_len = redir_files_and_count_argv_len(var, ptr, &cmd, 1);
	var->argv_for_cmd = malloc((argv_len + 1) * sizeof(char *));
	if (!var->argv_for_cmd)
		error_exit(var, "malloc failed: prepare_argv_and_redir\n");
	cpy_content_to_argv(var, var->argv_for_cmd, cmd, argv_len);
	find_path_to_cmd(var, &var->cmd, cmd->content, 0);
	close_pipes(var);
}

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
	signals_setup();
	return (1);
}
