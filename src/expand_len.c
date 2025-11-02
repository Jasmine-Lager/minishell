/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_len.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 13:13:18 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/02 00:51:14 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_keyword(char *str)
{
	int	len;

	len = 0;
	while (str[len] && ft_isalnum(str[len]))
	{
		len++;
	}
	return (len);
}

int	my_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

void	count_split(char *str, t_expand *exp)
{
	int	in_word;
	int	i;

	exp->len += ft_strlen(str);
	if (exp->dquote || !str)
		return ;
	in_word = 1;
	i = 0;
	while (str[i])
	{
		if (my_isspace(str[i]) && in_word)
		{
			in_word = 0;
			exp->nbr_split++;
		}
		else if (!my_isspace(str[i]) && !in_word)
		{
			in_word = 1;
		}
		i++;
	}
}

int	count_env_var_len(t_mini *var, char *str, int *i, t_expand *exp)
{
	char	*temp;
	int		key_len;

	(*i)++;
	if (str[*i] == '?')
	{
		temp = ft_itoa(var->exit_code);
		if (!temp)
			return (write(2, "minishell: malloc failed\n", 25), 0);
		exp->len += ft_strlen(temp);
	}
	else
	{
		key_len = len_keyword(&str[*i]);
		temp = malloc((key_len + 1) * sizeof(char));
		if (!temp)
			return (write(2, "minishell: malloc failed\n", 25), 0);
		ft_strlcpy(temp, &str[*i], key_len + 1);
		count_split(find_env_var(var->envp, temp), exp);
		*i += key_len - 1;
	}
	free(temp);
	return (1);
}

int	len_expanded(t_mini *var, char *str, t_expand *exp)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '"' && !exp->squote)
		{
			exp->can_be_rm = 0;
			exp->dquote = 1 - exp->dquote;
		}
		else if (str[i] == 39 && !exp->dquote)
		{
			exp->squote = 1 - exp->squote;
			exp->can_be_rm = 0;
		}
		else if (str[i] == '$' && !exp->squote
			&& (ft_isalnum(str[i + 1]) || str[i + 1] == '?'))
		{
			if (!count_env_var_len(var, str, &i, exp))
				return (0);
		}
		else
			exp->len++;
	}
	return (1);
}
