/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:10:05 by jasminelage       #+#    #+#             */
/*   Updated: 2025/11/03 21:59:45 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Copy environment variables so we can modify them
static char	**copy_envp(char **envp)
{
	int		count;
	int		i;
	char	**new_envp;

	count = 0;
	while (envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			while (i > 0)
				free(new_envp[--i]);
			free(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

static void	initialize_var(t_mini *var, char **envp)
{
	var->envp = copy_envp(envp);
	if (!var->envp)
	{
		write(2, "malloc in initialize_minishell failed\n", 38);
		free(var);
		exit(1);
	}
	var->line = NULL;
	var->tokens = NULL;
	var->nbr_pipes = 0;
	var->pipes = NULL;
	var->cmd = NULL;
	var->argv_for_cmd = NULL;
	var->nbr_heredoc = 0;
	var->exit_code = 0;
	var->term_saved = 1;
	if (tcgetattr(STDIN_FILENO, &(var->orig_term)) == -1)
		var->term_saved = 0;
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
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
	{
		write(2, "minishell: non-interactive mode not supported\n", 46);
		free(var);
		exit(1);
	}
	//handle SHLVL here
	initialize_var(var, envp);
	var->paths = ft_split(find_env_var(var->envp, "PATH"), ':');
	if (!var->paths)
		error_exit(var, "malloc in initialize_minishell failed\n");
}
