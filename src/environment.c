/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 13:00:41 by jasminelage       #+#    #+#             */
/*   Updated: 2025/11/06 16:11:42 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void copy_envp_without_index(char **old, char **new, int count, int skip)
{
	int i;
	int j;

	i = -1;
	j = 0;
	while (++i < count)
	{
		if (i != skip)
			new[j++] = old[i];
		else
			free(old[i]);
	}
	new[j] = NULL;
}

// Remove environment variable
int unset_env_var(t_mini *var, char *key)
{
	int		index;
	int		count;
	char	**new_envp;

	index = find_env_index(var->envp, key);
	if (index == -1)
		return (0);
	count = count_env_vars(var->envp);
	new_envp = malloc(sizeof(char *) * count);
	if (!new_envp)
		return (1);
	copy_envp_without_index(var->envp, new_envp, count, index);
	free(var->envp);
	var->envp = new_envp;
	return (0);
}

// Find index of environment variable by key
int	find_env_index(char **envp, char *key)
{
	int	i;
	int	len;

	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

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
