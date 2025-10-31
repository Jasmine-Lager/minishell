/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:10:50 by jasminelage       #+#    #+#             */
/*   Updated: 2025/10/31 16:58:33 by jlager           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Count environment variables
int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
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

// Update existing environment variable
int	update_env_var(t_mini *var, char *key, char *value)
{
	int		index;
	char	*new_var;
	char	*temp;

	index = find_env_index(var->envp, key);
	if (index == -1)
		return (set_env_var(var, key, value));
	temp = ft_strjoin(key, "=");
	if (!temp)
		return (1);
	new_var = ft_strjoin(temp, value);
	free(temp);
	if (!new_var)
		return (1);
	free(var->envp[index]);
	var->envp[index] = new_var;
	return (0);
}

// Set environment variable (add if doesn't exist, update if exists)
int	set_env_var(t_mini *var, char *key, char *value)
{
	int		index;
	int		count;
	char	**new_envp;
	char	*temp;

	index = find_env_index(var->envp, key);
	if (index != -1)
		return (update_env_var(var, key, value));
	count = count_env_vars(var->envp);
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return (1);
	index = -1;
	while (++index < count)
		new_envp[index] = var->envp[index];
	temp = ft_strjoin(key, "=");
	if (!temp)
		return (free(new_envp), 1);
	new_envp[index] = ft_strjoin(temp, value);
	free(temp);
	if (!new_envp[index])
		return (free(new_envp), 1);
	new_envp[index + 1] = NULL;
	free(var->envp);
	var->envp = new_envp;
	return (0);
}

// Remove environment variable
int	unset_env_var(t_mini *var, char *key)
{
	int		index;
	int		count;
	int		i;
	int		j;
	char	**new_envp;

	index = find_env_index(var->envp, key);
	if (index == -1)
		return (0);
	count = count_env_vars(var->envp);
	new_envp = malloc(sizeof(char *) * count);
	if (!new_envp)
		return (1);
	i = 0;
	j = 0;
	while (i < count)
	{
		if (i != index)
			new_envp[j++] = var->envp[i];
		else
			free(var->envp[i]);
		i++;
	}
	new_envp[j] = NULL;
	free(var->envp);
	var->envp = new_envp;
	return (0);
}
