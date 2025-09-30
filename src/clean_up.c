/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:38:35 by ksevciko          #+#    #+#             */
/*   Updated: 2025/09/30 13:32:23 by ksevciko         ###   ########.fr       */
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
	free(var->current->cmd);
	free(var->current->argv_for_cmd);
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
	var->current->delimiter = NULL;
	var->current->infile = NULL;
	var->current->outfile = NULL;
	free(var->pipes);
	var->pipes = NULL;
	free(var->current->cmd);
	var->current->cmd = NULL;
	free(var->current->argv_for_cmd);
	var->current->argv_for_cmd = NULL;
	var->current->append_mode = 0;
	var->current->here_doc = 0;
	var->current->delim_quoted = 0;
	var->nbr_pipes = 0;
}
