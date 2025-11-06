/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:10:50 by jasminelage       #+#    #+#             */
/*   Updated: 2025/11/06 16:11:34 by jasminelage      ###   ########.fr       */
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

static char *create_env_string(char *key, char *value)
{
	char *temp;
	char *result;
	
	temp = ft_strjoin(key, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, value);
	free(temp);
	return (result);
}

static void simple_copy_envp(char **old, char **new, int count)
{
	int i;

	i = -1;
	while (++i < count)
		new[i] = old[i];
}

// Set environment variable (add if doesn't exist, update if exists)
int set_env_var(t_mini *var, char *key, char *value)
{
	int		index;
	int		count;
	char	**new_envp;

	index = find_env_index(var->envp, key);
	if (index != -1)
		return (update_env_var(var, key, value));
	count = count_env_vars(var->envp);
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return (1);
	simple_copy_envp(var->envp, new_envp, count);
	new_envp[count] = create_env_string(key, value);
	if (!new_envp[count])
		return (free(new_envp), 1);
	new_envp[count + 1] = NULL;
	free(var->envp);
	var->envp = new_envp;
	return (0);
}

// Update existing environment variable
int update_env_var(t_mini *var, char *key, char *value)
{
	int		index;
	char	*new_var;

	index = find_env_index(var->envp, key);
	if (index == -1)
		return (set_env_var(var, key, value));
	new_var = create_env_string(key, value);
	if (!new_var)
		return (1);
	free(var->envp[index]);
	var->envp[index] = new_var;
	return (0);
}
