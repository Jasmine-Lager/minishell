/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:38:35 by ksevciko          #+#    #+#             */
/*   Updated: 2025/09/18 14:09:45 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// new
void	free_tokens(t_mini *var)
{
	t_token *ptr;
	t_token *temp;

	if (var == NULL)
		return;
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
//infile, outfile and argv for cmd
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

void free_one_line(t_mini *var)
{
	free(var->line);
	var->line = NULL;
	free_tokens(var);
	var->tokens = NULL;
	var->delimiter = NULL;
	var->infile = NULL;
	var->outfile = NULL;
	free(var->pipes);
	var->pipes = NULL;
	free(var->cmd);
	var->cmd = NULL;
	free(var->argv_for_cmd);
	var->argv_for_cmd = NULL;
	var->append_mode = 0;
	var->here_doc = 0;
	var->delim_quoted = 0;
	var->nbr_pipes = 0;
	var->exit_code = 0;
}
