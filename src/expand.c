/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:57 by jlager            #+#    #+#             */
/*   Updated: 2025/10/02 19:58:48 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// DO NOT expand anything in delimiter, do not do variable 
// substitution for delimiter
// DO mark if each token was quoted

#include "minishell.h"

int	cpy_env_var(t_mini *var, char *str, int *i, char *dst)
{
	int		len;
	char	*temp;
	int		key_len;

	key_len = 0;
	len = count_env_var_len(var, &str[*i], &key_len);
	(*i)++;
	if (str[*i] == '?')
	{
		temp = ft_itoa(var->exit_code);
		if (!temp)
			error_exit(var, BOLD RED "minishell: malloc failed\n" RESET);
		ft_strlcpy(dst, temp, len + 1);
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

char	*cpy_expanded(t_mini *var, char *str, char *result, bool delim)
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
		else if (str[i] == '$' && !squote && !delim
			&& (ft_isalnum(str[i + 1]) || str[i + 1] == '?'))
			j += cpy_env_var(var, str, &i, &result[j]);
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

// t_expand	*init_expanded(char *str)

char	*expand_str(t_mini *var, char *str) //todo: word splitting here, including detecting whethter the token cam be deleted (and only if it can = there are no "", run empty token in expand_tokens)
{
	int		len;
	char	*result;

	len = len_expanded(var, str);
	result = (char *)malloc((len + 1) * sizeof(char));
	if (!result)
		error_exit(var, BOLD RED "minishell: malloc failed\n" RESET);
	cpy_expanded(var, str, result, 0);
	return (result);
}

void	empty_token(t_mini *var, t_token *last, t_token **current,
		char *expanded)
{
	if ((*current)->type == CMD && (*current)->next
		&& (*current)->next->type == WORD)
		(*current)->next->type = CMD;
	if (!last)
	{
		var->tokens = (*current)->next;
		free(*current);
		*current = var->tokens;
	}
	else
	{
		last->next = (*current)->next;
		free(*current);
		*current = last->next;
	}
	free(expanded);
}

void	expand_tokens(t_mini *var)
{
	t_token	*current;
	t_token	*last;
	char	*expanded;

	current = var->tokens;
	last = NULL;
	while (current)
	{
		if (current->type == DELIMITER)
		{
			heredoc(var, current);
			current = current->next;
			continue ;
		}
		expanded = expand_str(var, current->content);
		free(current->content);
		if (!*expanded) //&& can_be_rm)
			empty_token(var, last, &current, expanded);
		else
		{
			current->content = expanded;
			last = current;
			current = current->next;
		}
	}
}
