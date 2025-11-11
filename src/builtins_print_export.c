/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_sort_export.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 13:35:25 by jlager            #+#    #+#             */
/*   Updated: 2025/11/11 13:39:29 by jlager           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// Print export without arguments (sorted list of all env vars)
void	print_export(t_mini *var)
{
	char	**sorted;
	char	*tmp;
	char	*equal_sign;
	int		i;
	int		j;

	i = 0;
	while (var->envp[i])
		i++;
	sorted = malloc(sizeof(char *) * (i + 1));
	if (!sorted)
		return ;
	i = -1;
	while (var->envp[++i])
		sorted[i] = var->envp[i];
	sorted[i] = NULL;
	i = -1;
	while (sorted[++i])
	{
		j = i;
		while (sorted[++j])
		{
			if (compare_env_vars(sorted[i], sorted[j]) > 0)
			{
				tmp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = tmp;
			}
		}
	}
	i = -1;
	while (sorted[++i])
	{
		write(1, "declare -x ", 11);
		equal_sign = ft_strchr(sorted[i], '=');
		if (equal_sign)
		{
			write(1, sorted[i], equal_sign - sorted[i] + 1);
			write(1, "\"", 1);
			write(1, equal_sign + 1, ft_strlen(equal_sign + 1));
			write(1, "\"", 1);
		}
		else
			write(1, sorted[i], ft_strlen(sorted[i]));
		write(1, "\n", 1);
	}
	free(sorted);
}

