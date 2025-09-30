/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:04:09 by ksevciko          #+#    #+#             */
/*   Updated: 2025/09/30 22:29:22 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	var->cmd = NULL;
	var->argv_for_cmd = NULL;
	var->exit_code = 0;
	var->paths = ft_split(find_env_var(envp, "PATH"), ':');
	if (!var->paths)
		error_exit(var, "malloc in initialize_minishell failed\n"); //can only be called after everything is initialized to null
}
