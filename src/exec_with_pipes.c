/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_with_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:59:49 by ksevciko          #+#    #+#             */
/*   Updated: 2025/09/30 11:12:34 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//for now coppied from pipex, needs to be modified for linked list of tokens

void	find_path(t_mini *var, char **path, char *cmd)
{
	char	*tmp;
	int		j;

	if (!cmd || !*cmd)
		error_exit(var, BOLD RED "minishell: invalid command\n" RESET); //TODO: check this whole function, it should not say pipex
	*path = ft_strdup(cmd);
	if (!*path)
		error_exit(var, BOLD RED "minishell: malloc failed\n" RESET);
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
			error_exit(var, BOLD RED "minishell: malloc failed\n" RESET);
		}
		free(tmp);
	}
	if (access(*path, X_OK) == -1)
		command_not_found(var, path);
}

void	cpy_content_to_argv(t_mini *var, char **dst_argv, t_token *ptr,
			size_t argv_len)
{
	size_t	i;

	i = 0;
	if (!dst_argv)
		error_exit(var, "malloc failed: find_nth_cmd_and_argv\n");
	while (i < argv_len)
	{
		if (ptr->type == CMD || ptr->type == WORD || ptr->type == FLAG)
		{
			dst_argv[i] = ptr->content;
			i++;
		}
		ptr = ptr->next;
	}
	dst_argv[i] = NULL;
	find_path(var, &var->cmd, var->argv_for_cmd[0]);
}

void	find_nth_cmd_and_argv(t_mini *var, int cmd_n)
{
	int	i;
	t_token	*ptr;
	t_token *tmp;
	int	argv_len;

	i = 0;
	ptr = var->tokens;
	while (ptr && (i < cmd_n || ptr->type != CMD))
	{
		if (ptr->type == CMD)
			i++;
		ptr = ptr->next;
	}
	if (!ptr)
		error_exit(var, "missing a command\n");
	argv_len = 1;
	tmp = ptr->next;
	while (tmp && (tmp->type != CMD))
	{
		if (tmp->type == FLAG || tmp->type == WORD)
			argv_len++;
		tmp = tmp->next;
	}
	var->argv_for_cmd = malloc((argv_len + 1) * sizeof(char *));
	cpy_content_to_argv(var, var->argv_for_cmd, ptr, argv_len);
}

void	wait_for_children(t_mini *var, pid_t last_child_pid)
{
	int	i;
	int	status;

	i = 0;
	while (i <= var->nbr_pipes)
	{
		if (wait(&status) == last_child_pid)
		{
			if (WIFEXITED(status))
				var->exit_code = WEXITSTATUS(status);
			else
				var->exit_code = 1;
		}
		i++;
	}
}

void	execute_cmds(t_mini *var)
{
	int		n;
	pid_t	pid;

	n = var->here_doc;
	while (n <= var->nbr_pipes)
	{
		pid = fork();
		if (pid == -1) 
		{
			perror("fork");
			free_var_exit(var, 1);
		}
		else if (pid == 0)
		{
			redirect_for_pipes(var, n);
			find_nth_cmd_and_argv(var, n - var->here_doc);
			close_pipes(var);
			execve(var->cmd, var->argv_for_cmd, var->envp);
			perror("execve");
			free_var_exit(var, 1);
		}
		n++;
	}
	close_pipes(var);
	wait_for_children(var, pid);
}

// The executor creates processes, establishes pipes, handles redirections,
// and manages command execution.This stage orchestrates the actual work.

// Execution Components :

// Process creation with
// fork() / execve()
// Pipe establishment and management
// File descriptor redirection
// Built in command detection and handling
// 	Step-by-Step Implementation Approach