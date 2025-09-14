/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_no_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:22:26 by ksevciko          #+#    #+#             */
/*   Updated: 2025/09/14 20:22:26 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_no_pipes(t_mini *var)
{
	int fd0;
	int	fd1;

	if (var->infile)
		fd0 = open(var->infile, O_RDONLY | O_CREAT, 0644);
	else
		fd0 = 0;
	if (fd0 == -1)
	{
		perror("failed to open input");
		free_var_exit(var, 1);
	}
	if (!var->append_mode)
		fd1 = open(var->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd1 = open(var->outfile, O_WRONLY | O_CREAT | O_APPEND , 0644);
	if (fd1 == -1)
	{
		perror("failed to open output");
		free_var_exit(var, 1);
	}
	if (dup2(fd0, 0) == -1 || dup2(fd1, 1) == -1)
	{
		dup2_error(var);
	}
	close(fd0);
	close(fd1);
}

void	execute_cmd(t_mini *var)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1) 
	{
		perror("fork");
		free_var_exit(var, 1);
	}
	else if (pid == 0)
	{
		// redirect_no_pipes(var);
		find_nth_cmd_and_argv(var, 0);
		execve(var->cmd, var->argv_for_cmd, var->envp);
		perror("execve");
		free_var_exit(var, 1);
	}
	wait_for_children(var, pid);
}
