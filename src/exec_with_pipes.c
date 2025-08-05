/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:59:49 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/05 09:38:25 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//for now coppied from pipex, needs to be modified for linked list of tokens

void	find_path(t_mini *var, char **path, char *cmd) //has to change a lot
{
	char	*tmp;
	int		j;

	if (!cmd || !*cmd)
		other_error(var, "pipex: invalid command\n");
	*path = ft_strdup(cmd);
	if (!*path)
		other_error(var, "pipex: malloc failed\n");
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
			other_error(var, "pipex: malloc failed\n");
		}
		free(tmp);
	}
	if (access(*path, X_OK) == -1)
		command_not_found(var, path);
}

void	cmds_to_struct(t_mini *var) //needs to be fully rewritten
{
	// var->argv_for_cmd = ft_split(cmd, ' ');
	// if (!var->argv_for_cmd)
	// 	other_error(var, "pipex: malloc failed\n");
	var->cmd = NULL;
	find_path(var, &var->cmd, var->argv_for_cmd[0]);
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
	int		i;
	pid_t	pid;

	i = 0;
	while (i <= var->nbr_pipes)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free_var_exit(var, 1);
		}
		else if (pid == 0)
		{
			redirect_for_pipes(var, i);
			cmds_to_struct(var);
			close_pipes(var);
			execve(var->cmd, var->argv_for_cmd, var->envp);
			perror("execve");
			free_var_exit(var, 1);
		}
		i++;
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