/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 00:00:29 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/30 15:29:58 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cpy_expanded_delim(char *str, char *result)
{
	int	dquote;
	int	squote;
	int	i;
	int	j;

	dquote = 0;
	squote = 0;
	i = -1;
	j = 0;
	while (str[++i])
	{
		if (str[i] == '"' && !squote)
			dquote = 1 - dquote;
		else if (str[i] == 39 && !dquote)
			squote = 1 - squote;
		else
			result[j++] = str[i];
	}
	result[j] = '\0';
	return (result);
}

int	len_expanded_heredoc(t_mini *var, char *str)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '?'))
			len += count_env_var_len(var, str, &i);
		else
			len++;
		i++;
	}
	return (len);
}

char	*cpy_expanded_heredoc(t_mini *var, char *str, char *result)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '?'))
			j += cpy_env_var(var, str, &i, &result[j]);
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	*expand_heredoc(t_mini *var, char *str)
{
	int		len;
	char	*result;

	len = len_expanded_heredoc(var, str);
	result = (char *)malloc((len + 1) * sizeof(char));
	if (!result)
		error_exit(var, "minishell: malloc failed\n");
	cpy_expanded_heredoc(var, str, result);
	return (result);
}
