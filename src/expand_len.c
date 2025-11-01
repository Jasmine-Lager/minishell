/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_len.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 13:13:18 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/01 17:54:28 by ksevciko         ###   ########.fr       */
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

int	count_split(char *expanded, t_split *split)
{
	int	in_word;

	if (split->dquote && expanded)
		return (0);
	in_word = 1;
	return (in_word);
}

int	count_env_var_len(t_mini *var, char *str, int *i, t_split *split)
{
	int		len;
	char	*temp;
	int		key_len;

	(*i)++;
	if (str[*i] == '?')
	{
		temp = ft_itoa(var->exit_code);
		if (!temp)
			return (write(2, "minishell: malloc failed\n", 25), -1);
		len = ft_strlen(temp);
	}
	else
	{
		key_len = len_keyword(&str[*i]);
		temp = malloc((key_len + 1) * sizeof(char));
		if (!temp)
			return (write(2, "minishell: malloc failed\n", 25), -1);
		ft_strlcpy(temp, &str[*i], key_len + 1);
		len = ft_strlen(find_env_var(var->envp, temp));
		split->nbr_split = count_split(find_env_var(var->envp, temp), split);
		*i += key_len - 1;
	}
	free(temp);
	return (len);
}

int	len_expanded(t_mini *var, char *str, t_split *split)
{
	int	len;
	int	i;
	int	check;

	split->dquote = 0;
	split->squote = 0;
	len = 0;
	i = -1;
	check = 0;
	while (str[++i])
	{
		if (str[i] == '"' && !split->squote)
		{
			split->can_be_rm = 0;
			split->dquote = 1 - split->dquote;
		}
		else if (str[i] == 39 && !split->dquote)
		{
			split->squote = 1 - split->squote;
			split->can_be_rm = 0;
		}
		else if (str[i] == '$' && !split->squote
			&& (ft_isalnum(str[i + 1]) || str[i + 1] == '?'))
		{
			check = count_env_var_len(var, str, &i, split);
			len += check;
		}
		else
			len++;
		if (check == -1)
			return (-1);
	}
	return (len);
}
