/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:04:09 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/13 13:51:40 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_minishell(t_mini *var, int argc, char **argv, char **envp)
{
	if (!var)
	{
		write(2, "malloc in main.c failed\n", 25);
		exit(1);
	}
	var->envp = envp;
	var->tokens = NULL;
	var->infile = NULL;
	var->outfile = NULL;
	var->append_mode = 0;
	var->here_doc = 0;
	var->delimiter = NULL;
	var->nbr_pipes = 0;
	var->pipes = NULL;
	var->cmd = NULL;
	var->argv_for_cmd = NULL;
	var->exit_code = 0;
	var->paths = ft_split(find_env_var(envp, "PATH"), ':');
	if (!var->paths)
		error_exit(var, "malloc in initialize_minishell failed\n"); //can only be called after everything is initialized to null
	if (argc != 1 || !*argv) //only here to silence error unused parameter
		error_exit(var, "minishell: wrong number of arguments\n");
}

// void initialize_minishell(t_mini *var, int argc, char **argv, char **envp)
// {
// 	char *path_value;

// 	if (!var)
// 		return_error("malloc in main.c failed\n", 1) // would need shortening
// 	var->envp = envp;
// 	var->tokens = NULL;
// 	var->infile = NULL;
// 	var->outfile = NULL;
// 	var->append_mode = 0;
// 	var->here_doc = 0;
// 	var->delimiter = NULL;
// 	var->nbr_pipes = 0;
// 	var->pipes = NULL;
// 	var->cmd = NULL;
// 	var->argv_for_cmd = NULL;
// 	var->exit_code = 0;
// 	path_value = find_env_var(envp, "PATH");
// 	if (!path_value)
// 		path_value = "/usr/bin:/bin"; // default PATH if PATH not set
// 	var->paths = ft_split(path_value, ':');
// 	if (!var->paths)
// 		error_exit(var, "malloc in initialize_minishell failed\n"); // can only be called after everything is initialized to null
// 	if (argc != 1 || !*argv) // only here to silence error unused parameter
// 		error_exit(var, "minishell: wrong number of arguments\n");
// }
