/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:04:09 by ksevciko          #+#    #+#             */
/*   Updated: 2025/09/30 13:03:22 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_current(t_mini *var)
{
	var->current = malloc(sizeof(t_cmd));
	if (!var->current)
	{
		write(2, "malloc in initialize_current faied\n", 35);
		free(var);
		exit(1);
	}
	var->current->cmd = NULL;
	var->current->argv_for_cmd = NULL;
	var->current->infile = NULL;
	var->current->outfile = NULL;
	var->current->append_mode = 0;
	var->current->here_doc = 0;
	var->current->delimiter = NULL;
	var->current->delim_quoted = 0;
}

void	initialize_minishell(t_mini *var, int argc, char **argv, char **envp)
{
	if (!var)
	{
		write(2, "malloc in main.c failed\n", 24);
		exit(1);
	}
	if (argc != 1 || !*argv)
	{
		write(2, "minishell: wrong number of arguments\n", 37);
		free(var);
		exit(2);
	}
	var->envp = envp;
	var->line = NULL;
	var->tokens = NULL;
	var->nbr_pipes = 0;
	var->pipes = NULL;
	initialize_current(var);
	var->exit_code = 0;
	var->paths = ft_split(find_env_var(envp, "PATH"), ':');
	if (!var->paths)
		error_exit(var, "malloc in initialize_minishell failed\n"); //can only be called after everything is initialized to null
}
