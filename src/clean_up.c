/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:38:35 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/07 14:35:08 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_mini *var)
{
	t_token	*ptr;
	t_token	*temp;

	if (!var || !var->tokens)
		return ;
	ptr = var->tokens;
	while (ptr)
	{
		free(ptr->content);
		temp = ptr;
		ptr = ptr->next;
		free(temp);
	}
	var->tokens = NULL;
}

//cannot free var->delimiter here, because it is a pointer to the same thing as tokens->content, and we free that instead, the same thing with infile and outfile
void	free_var_exit(t_mini *var, int exit_code)
{
	if (var && var->paths)
		free_arr(var->paths);
	free(var->line);
	free_tokens(var);
	free(var->pipes);
	free(var->cmd);
	if (var && var->argv_for_cmd)
		free_arr(var->argv_for_cmd);
	free(var);
	rl_clear_history();
	exit (exit_code);
}

void	free_one_line(t_mini *var)
{
	free(var->line);
	var->line = NULL;
	free_tokens(var);
	free(var->pipes);
	var->pipes = NULL;
	free(var->cmd);
	var->cmd = NULL;
	if (var && var->argv_for_cmd)
		free_arr(var->argv_for_cmd);
}

//the string says what message the error should display
void	other_error(t_mini *var, char *str)
{
	write(2, str, ft_strlen(str));
	free_var_exit(var, 1);
}

void	command_not_found(t_mini *var, char **path)
{
	free(*path);
	*path = NULL;
	perror("command not found");
	free_var_exit(var, 127);
}

void	dup2_error(t_mini *var)
{
	perror("dup2");
	free_var_exit(var, 1);
}
