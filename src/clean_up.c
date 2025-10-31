/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:10:37 by jasminelage       #+#    #+#             */
/*   Updated: 2025/10/31 17:03:57 by jlager           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// new
void	free_tokens(t_mini *var)
{
	t_token	*ptr;
	t_token	*temp;

	if (var == NULL)
		return ;
	ptr = var->tokens;
	while (ptr != NULL)
	{
		temp = ptr->next;
		free(ptr->content);
		free(ptr);
		ptr = temp;
	}
	var->tokens = NULL;
}

// Free environment variables array
static void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

//cannot free var->delimiter here, because it is a pointer to the same 
//thing as tokens->content, and we free that instead, the same thing with 
//infile, outfile and strings in argv_for_cmd
void	free_var_exit(t_mini *var, int exit_code)
{
	if (var && var->paths)
		free_arr(var->paths);
	free(var->line);
	free_tokens(var);
	free(var->pipes);
	free(var->cmd);
	free(var->argv_for_cmd);
	if (var && var->envp)
		free_envp(var->envp);
	free(var);
	rl_clear_history();
	exit(exit_code);
}

void	free_one_line(t_mini *var)
{
	free(var->line);
	var->line = NULL;
	free_tokens(var);
	var->tokens = NULL;
	free(var->pipes);
	var->pipes = NULL;
	free(var->cmd);
	var->cmd = NULL;
	free(var->argv_for_cmd);
	var->argv_for_cmd = NULL;
	var->nbr_pipes = 0;
	var->nbr_heredoc = 0;
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
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGINT)
					var->exit_code = 130;
				else if (WTERMSIG(status) == SIGQUIT)
					var->exit_code = 131;
				else
					var->exit_code = 128 + WTERMSIG(status);
			}
			else
				var->exit_code = 1;
		}
		i++;
	}
}
