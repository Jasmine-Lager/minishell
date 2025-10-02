/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_len.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 13:13:18 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/02 11:10:02 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_keyword(char *str)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != '"' && str[len] != 39 && str[len] != '$'
			&& str[len] != ' ')
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
		else if (str[i] == '$' && !squote  && (ft_isalnum(str[i + 1]) || str[i + 1] == '?'))
			len += count_env_var_len(var, str, &i);
		else
			len++;
		i++;
	}
	return (len);
}
