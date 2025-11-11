/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_print_export.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 13:35:25 by jlager            #+#    #+#             */
/*   Updated: 2025/11/11 13:55:13 by jlager           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Create array of pointers to env vars
static char	**create_sorted_array(char **envp)
{
	char	**sorted;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	sorted = malloc(sizeof(char *) * (i + 1));
	if (!sorted)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		sorted[i] = envp[i];
		i++;
	}
	sorted[i] = NULL;
	return (sorted);
}

// Compare two environment variable strings for sorting
static int	compare_env_vars(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] != '=' && s2[i] != '=')
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	if (s1[i] == '=' && s2[i] != '=')
		return (-1);
	if (s1[i] != '=' && s2[i] == '=')
		return (1);
	return (s1[i] - s2[i]);
}

// Sort array of pointers using selection sort
static void	sort_env_array(char **sorted)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	while (sorted[i])
	{
		j = i + 1;
		while (sorted[j])
		{
			if (compare_env_vars(sorted[i], sorted[j]) > 0)
			{
				tmp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

// Print single env var in export format
static void	print_export_var(char *env_var)
{
	char	*equal_sign;

	write(1, "declare -x ", 11);
	equal_sign = ft_strchr(env_var, '=');
	if (equal_sign)
	{
		write(1, env_var, equal_sign - env_var + 1);
		write(1, "\"", 1);
		write(1, equal_sign + 1, ft_strlen(equal_sign + 1));
		write(1, "\"", 1);
	}
	else
		write(1, env_var, ft_strlen(env_var));
	write(1, "\n", 1);
}

// Print export without arguments (sorted list of all env vars)
void	print_export(t_mini *var)
{
	char	**sorted;
	int		i;

	sorted = create_sorted_array(var->envp);
	if (!sorted)
		return ;
	sort_env_array(sorted);
	i = 0;
	while (sorted[i])
	{
		print_export_var(sorted[i]);
		i++;
	}
	free(sorted);
}
