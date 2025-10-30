/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:11:00 by jasminelage       #+#    #+#             */
/*   Updated: 2025/10/30 15:21:59 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	find_path_to_cmd(t_mini *var, char **path, char *cmd)
{
	char	*tmp;
	int		j;

	if (!cmd || !*cmd)
		error_exit(var, "minishell: invalid command\n");
	// Check if it's a built-in first
	if (is_builtin(cmd))
	{
		*path = ft_strdup(cmd);
		if (!*path)
			error_exit(var, "minishell: malloc failed\n");
		return ;
	}
	*path = ft_strdup(cmd);
	if (!*path)
		error_exit(var, "minishell: malloc failed\n");
	j = 0;
	while (access(*path, X_OK) == -1 && var->paths && var->paths[j])
	{
		free(*path);
		tmp = ft_strjoin(var->paths[j++], "/");
		*path = ft_strjoin(tmp, cmd);
		if (!tmp || !*path)
		{
			free(tmp);
			free(*path);
			*path = NULL;
			error_exit(var, "minishell: malloc failed\n");
		}
		free(tmp);
	}
	if (access(*path, X_OK) == -1 && !is_builtin(cmd))
		command_not_found(var, path);
}

void	cpy_content_to_argv(t_mini *var, char **dst_argv, t_token *ptr,
			size_t argv_len)
{
	size_t	i;

	if (ptr->type != CMD) //should never happen
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
	find_path_to_cmd(var, &var->cmd, cmd->content);
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
static bool	execute_single_builtin(t_mini *var)
{
	t_token	*ptr;
	t_token	*cmd;
	int		argv_len;
	int		saved_stdin;
	int		saved_stdout;

	// Save original stdin/stdout
	saved_stdin = dup(0);
	saved_stdout = dup(1);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		perror("dup");
		return (0);
	}
	ptr = var->tokens;
	cmd = NULL;
	argv_len = redir_files_and_count_argv_len(var, ptr, &cmd, 1);
	var->argv_for_cmd = malloc((argv_len + 1) * sizeof(char *));
	if (!var->argv_for_cmd)
	{
		close(saved_stdin);
		close(saved_stdout);
		write(2, "malloc failed: execute_single_builtin\n", 38);
		return (0);
	}
	cpy_content_to_argv(var, var->argv_for_cmd, cmd, argv_len);
	
	// Execute built-in
	var->exit_code = execute_builtin(var, var->argv_for_cmd);
	
	// Restore stdin/stdout
	dup2(saved_stdin, 0);
	dup2(saved_stdout, 1);
	close(saved_stdin);
	close(saved_stdout);
	
	return (1);
}

bool	execute_cmds(t_mini *var)
{
	int		n;
	pid_t	pid;

	// Check if single command is a built-in
	if (var->nbr_pipes == 0 && var->tokens && var->tokens->type == CMD
		&& is_builtin(var->tokens->content))
	{
		return (execute_single_builtin(var));
	}
	if (!create_pipes(var))
		return (0);
	signals_execution();  // Change signal handling during execution
	n = -1;
	while (++n <= var->nbr_pipes)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			signals_setup();  // Restore interactive signals
			return (0);
		}
		else if (pid == 0)
		{
			signals_child();  // Child uses default signal handling
			prepare_argv_and_redir(var, n);
			// Check if it's a built-in
			if (is_builtin(var->argv_for_cmd[0]))
				execute_builtin_child(var);
			// Otherwise execute as external command
			execve(var->cmd, var->argv_for_cmd, var->envp);
			perror("execve");
			free_var_exit(var, 1);
		}
	}
	close_pipes(var);
	wait_for_children(var, pid);
	signals_setup();  // Restore interactive signals after execution
	return (1);
}
