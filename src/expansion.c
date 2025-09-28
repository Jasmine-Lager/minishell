/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:57 by jlager            #+#    #+#             */
/*   Updated: 2025/09/28 01:41:16 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// The expander handles variable substitution, tilde expansion,
// and quote removal before execution. This stage processes $HOME, $?,
// and environment variables.

// Expansion Tasks (Variable Processing):
// Environment variable substitution ($VAR)  - use find_env_var from initialize_var.c
//DO NOT expand anything in delimiter, do not do variable substitution for delimiter
// DO mark if each token was quoted
// Exit status expansion ($?)
// Quote removal after expansion

#include "minishell.h"

int	len_keyword(char *str)
{
	int len;

	len = 0;
	while (str[len] && str[len] != '"' && str[len] != 39 && str[len] != '$')
	{
		len++;
	}
	return (len);
}

int	count_env_var_len(t_mini *var, char *str, int *i)
{
	int	len;
	char *temp;
	int	key_len;

	(*i)++;
	if (str[*i] == '?')
	{
		temp = ft_itoa(var->exit_code);
		if (!temp)
			error_exit(var, BOLD RED "minishell: malloc failed\n" RESET);
		len = ft_strlen(temp);
	}
	else
	{
		key_len = len_keyword(&str[*i]);
		temp = malloc((key_len + 1) * sizeof(char));
		if (!temp)
			error_exit(var, BOLD RED "minishell: malloc failed\n" RESET);
		ft_strlcpy(temp, &str[*i], key_len + 1);
		len = ft_strlen(find_env_var(var->envp, temp));
		*i += key_len - 1;
	}
	free(temp);
	return (len);
}

int	len_expanded(t_mini *var, char *str)
{
	int	len;
	int	dquote;
	int	squote;
	int	i;

	len = 0;
	dquote = 0;
	squote = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '"' && !squote)
			dquote = 1 - dquote;
		else if (str[i] == 39 && !dquote)
			squote = 1 - squote;
		else if (str[i] == '$' && !squote)
			len += count_env_var_len(var, str, &i);
		else
			len++;
		i++;
	}
	return (len);
}

int cpy_env_var(t_mini *var, char *str, int *i, char *dst)
{
	int	len;
	char *temp;
	int	key_len;

	key_len = 0;
	len = count_env_var_len(var, &str[*i], &key_len);
	(*i)++;
	if (str[*i] == '?')
	{
		temp = ft_itoa(var->exit_code);
		if (!temp)
			error_exit(var, BOLD RED "minishell: malloc failed\n" RESET);
		ft_strlcpy(dst, temp, len);
	}
	else
	{
		key_len = len_keyword(&str[*i]);
		temp = malloc((key_len + 1) * sizeof(char));
		if (!temp)
			error_exit(var, BOLD RED "minishell: malloc failed\n" RESET);
		ft_strlcpy(temp, &str[*i], key_len + 1);
		ft_strlcpy(dst, find_env_var(var->envp, temp), len + 1);
		*i += key_len - 1;
	}
	return (free(temp), len);
}

char	*cpy_expanded(t_mini *var, char *str, char *result)
{
	int	dquote;
	int	squote;
	int	i;
	int	j;

	dquote = 0;
	squote = 0;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"' && !squote)
			dquote = 1 - dquote;
		else if (str[i] == 39 && !dquote)
			squote = 1 - squote;
		else if (str[i] == '$' && !squote)
			j += cpy_env_var(var, str, &i, &result[j]);
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	*expand_str(t_mini *var, char *str)
{
	int		len;
	char	*result;

	len = len_expanded(var, str);
	result = (char *)malloc((len + 1) * sizeof(char));
	if (!result)
		error_exit(var, BOLD RED "minishell: malloc failed\n" RESET);
	cpy_expanded(var, str, result);
	return (result);
}

void	expand_tokens(t_mini *var)
{
	t_token	*current;
	char	*expanded;

	current = var->tokens;
	while (current)
	{
		if (current->type == DELIMITER)
		{
			current = current->next;
			continue;
		}
		expanded = expand_str(var, current->content);
		if (expanded)
		{
			free(current->content);
			current->content = expanded;
		}
		current = current->next;
	}
}


// // Replace $VAR with its value in a string
// char *expand_single_variable(t_mini *var, char *str, int *pos)
// {
// 	int start;
// 	int end;
// 	char *var_value;
// 	char *result;

// 	start = *pos + 1;
// 	end = find_var_name_end(str, start);
// 	var_value = get_variable_value(var, str, start, end);
// 	if (!var_value)
// 		return (NULL);
// 	if (end <= start)
// 		end = start;
// 	result = build_result_string(str, *pos, end, var_value);
// 	if (result)
// 		*pos = *pos + ft_strlen(var_value);
// 	free(var_value);
// 	return (result);
// }

// // Main expansion function for a single string
// char *expand_variables_in_string(t_mini *var, char *str, t_quotes quotes)
// {
// 	char *result;
// 	char *new_str;
// 	int pos;

// 	pos = 0;
// 	if (!str || quotes == SINGLE)
// 		return (str ? ft_strdup(str) : NULL);
// 	result = ft_strdup(str);
// 	if (!result)
// 		return (NULL);
// 	while (result[pos])
// 	{
// 		if (result[pos] == '$' && result[pos + 1])
// 		{
// 			new_str = expand_single_variable(var, result, &pos);
// 			if (!new_str)
// 				return (free(result), NULL);
// 			free(result);
// 			result = new_str;
// 		}
// 		else
// 			pos++;
// 	}
// 	return (result);
// }

// // Process all tokens
// void expand_tokens(t_mini *var)
// {
// 	t_token *current = var->tokens;
// 	char *expanded;

// 	while (current)
// 	{
// 		if (current->type == DELIMITER)
// 		{
// 			current = current->next;
// 			continue;
// 		}
// 		expanded = expand_variables_in_string(var, current->content, current->quotes);
// 		if (expanded)
// 		{
// 			free(current->content);
// 			current->content = expanded;
// 		}
// 		current = current->next;
// 	}
// }
