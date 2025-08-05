/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:38:35 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/05 08:54:20 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_var_exit(t_mini *var, int exit_code)
{
	if (var && var->paths)
		free_arr(var->paths);
	free(var->line);
	//free tokens!!
	free(var->infile);
	free(var->outfile);
	free(var->pipes);
	free(var->cmd);
	if (var && var->argv_for_cmd)
		free_arr(var->argv_for_cmd);
	free(var);
	exit (exit_code);
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
