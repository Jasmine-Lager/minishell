/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rm_quotes_delim.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 18:38:24 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/11 14:01:24 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_no_quotes(char *delim, bool *quoted, int dquote, int squote)
{
	int	len;
	int	i;

	len = 0;
	i = -1;
	while (delim[++i])
	{
		if (delim[i] == '"' && !squote)
		{
			dquote = 1 - dquote;
			*quoted = 1;
		}
		else if (delim[i] == 39 && !dquote)
		{
			squote = 1 - squote;
			*quoted = 1;
		}
		else if (!ft_isalnum(delim[i]) && delim[i] != '_')
			return (write(2, "invalid character in delimiter", 30), -1);
		else
			len++;
	}
	return (len);
}

char	*cpy_no_quotes(char *str, char *result)
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

bool	rm_quotes_delim(t_mini *var, char **delim, t_expand *exp)
{
	int		len;
	char	*result;
	bool	quoted;

	quoted = 0;
	len = len_no_quotes(*delim, &quoted, 0, 0);
	if (len == -1)
	{
		close(exp->fd);
		free(exp);
		free_var_exit(var, 1);
	}
	result = (char *)malloc((len + 1) * sizeof(char));
	if (!result)
	{
		close(exp->fd);
		free(exp);
		error_exit(var, "minishell: malloc failed\n");
	}
	cpy_no_quotes(*delim, result);
	free(*delim);
	*delim = result;
	return (quoted);
}
