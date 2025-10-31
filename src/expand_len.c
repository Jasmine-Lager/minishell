/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_len.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 13:13:18 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/31 15:34:53 by ksevciko         ###   ########.fr       */
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

int	count_env_var_len(t_mini *var, char *str, int *i)
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
		*i += key_len - 1;
	}
	free(temp);
	return (len);
}

int	len_expanded(t_mini *var, char *str, int dquote, int squote)
{
	int	len;
	int	i;
	int	check;

	len = 0;
	i = -1;
	check = 0;
	while (str[++i])
	{
		if (str[i] == '"' && !squote)
			dquote = 1 - dquote;
		else if (str[i] == 39 && !dquote)
			squote = 1 - squote;
		else if (str[i] == '$' && !squote
			&& (ft_isalnum(str[i + 1]) || str[i + 1] == '?'))
		{
			check = count_env_var_len(var, str, &i);
			len += check;
		}
		else
			len++;
		if (check == -1)
			return (-1);
	}
	return (len);
}
