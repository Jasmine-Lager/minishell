/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:38:35 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/07 12:51:19 by jasminelage      ###   ########.fr       */
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
				// Check if terminated by signal
				if (WTERMSIG(status) == SIGINT)
					var->exit_code = 130;  // Ctrl+C
				else if (WTERMSIG(status) == SIGQUIT)
					var->exit_code = 131;  // Ctrl+backslash
				else
					var->exit_code = 128 + WTERMSIG(status);
			}
			else
				var->exit_code = 1;
		}
		i++;
	}
}
