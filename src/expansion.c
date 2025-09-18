/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:57 by jlager            #+#    #+#             */
/*   Updated: 2025/09/18 12:34:27 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// The expander handles variable substitution, tilde expansion,
// and quote removal before execution. This stage processes $HOME, $?,
// and environment variables.

// Expansion Tasks (Variable Processing):
// Environment variable substitution ($VAR)  - use find_env_var from initialize_var.c
// DO NOT expand anything in delimiter, do not do variable substitution for delimiter
// DO mark if each token was quoted
// Exit status expansion ($?)
// Home directory expansion (~)
// Quote removal after expansion

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

// Get the value of a variable
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

// Helper function to extract and get variable value
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

// Replace $VAR with its value in a string
char *expand_single_variable(t_mini *var, char *str, int *pos)
{
	int start;
	int end;
	char *var_value;
	char *result;

	start = *pos + 1;
	end = find_var_name_end(str, start);
	var_value = get_variable_value(var, str, start, end);
	if (!var_value)
		return (NULL);
	if (end <= start)
		end = start;
	result = build_result_string(str, *pos, end, var_value);
	if (result)
		*pos = *pos + ft_strlen(var_value);
	free(var_value);
	return (result);
}

// Main expansion function for a single string
char *expand_variables_in_string(t_mini *var, char *str, t_quotes quotes)
{
	char *result;
	char *new_str;
	int pos;

	pos = 0;
	if (!str || quotes == SINGLE)
		return (str ? ft_strdup(str) : NULL);
	result = ft_strdup(str);
	if (!result)
		return (NULL);
	while (result[pos])
	{
		if (result[pos] == '$' && result[pos + 1])
		{
			new_str = expand_single_variable(var, result, &pos);
			if (!new_str)
				return (free(result), NULL);
			free(result);
			result = new_str;
		}
		else
			pos++;
	}
	return (result);
}

// Process all tokens
void expand_tokens(t_mini *var)
{
	t_token *current = var->tokens;
	char *expanded;

	while (current)
	{
		if (current->type == DELIMITER)
		{
			current = current->next;
			continue;
		}
		expanded = expand_variables_in_string(var, current->content, current->quotes);
		if (expanded)
		{
			free(current->content);
			current->content = expanded;
		}
		current = current->next;
	}
}
