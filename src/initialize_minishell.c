/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:04:09 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/06 21:36:46 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_env_var(char **envp, char *key)
{
	int	i;
	int	len;

	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	initialize_minishell(t_mini *var, int argc, char **argv, char **envp)
{
	if (!var)
	{
		write(2, "malloc in main.c failed\n", 25); //could be shortened with function return_error, but that function would have to be only used here specifically, because it could not call free_var_exit, and everywhere else it needs to be called. i think it would reduce readability.
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
		other_error(var, "malloc in initialize_minishell failed\n"); //can only be called after everything is initialized to null
	if (argc != 1 || !*argv) //only here to silence error unused parameter
		other_error(var, "minishell: wrong number of arguments\n");
}
