/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:49:38 by jasminelage       #+#    #+#             */
/*   Updated: 2025/09/18 13:52:09 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Find the end of a variable name after $
int find_var_name_end(char *str, int start)
{
	int i = start;

	if (str[i] == '?')
		return (i + 1);
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (start);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;

	return (i);
}

// Extract variable name from $VAR
char *extract_var_name(char *str, int start, int end)
{
	char *var_name;
	int len = end - start;

	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);

	ft_strlcpy(var_name, &str[start], len + 1);
	return (var_name);
}

// Helper function to extract and get variable value
char *get_var_value(t_mini *var, char *var_name)
{
	char *value;

	if (!var_name)
		return (ft_strdup(""));
	if (ft_strncmp(var_name, "?", 2) == 0)
	{
		value = ft_itoa(var->exit_code);
		return (value ? value : ft_strdup(""));
	}
	value = find_env_var(var->envp, var_name);
	return (value ? ft_strdup(value) : ft_strdup(""));
}

// Extract and get variable value
char *get_variable_value(t_mini *var, char *str, int start, int end)
{
	char *var_name;
	char *var_value;

	if (end <= start)
		return (ft_strdup("$"));
	var_name = extract_var_name(str, start, end);
	if (!var_name)
		return (NULL);
	var_value = get_var_value(var, var_name);
	free(var_name);
	return (var_value);
}

// Helper function to build result string
char *build_result_string(char *str, int pos, int end, char *var_value)
{
	char *before;
	char *after;
	char *temp;
	char *result;

	before = ft_substr(str, 0, pos);
	after = ft_strdup(&str[end]);
	if (!before || !after)
	{
		free(before);
		free(after);
		return (NULL);
	}
	temp = ft_strjoin(before, var_value);
	result = NULL;
	if (temp)
		result = ft_strjoin(temp, after);
	free(before);
	free(after);
	free(temp);
	return (result);
}

