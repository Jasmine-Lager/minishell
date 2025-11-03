/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 00:00:29 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/03 21:41:03 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_env_var_heredoc(t_mini *var, char *str, int *i)
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

int	cpy_env_var_heredoc(t_mini *var, char *str, int *i, char *dst)
{
	int		len;
	char	*tmp;
	int		key_len;

	key_len = 0;
	len = len_env_var_heredoc(var, &str[*i], &key_len);
	(*i)++;
	if (str[*i] == '?')
	{
		tmp = ft_itoa(var->exit_code);
		if (!tmp || len == -1)
			return (free(tmp), write(2, "minishell: malloc failed\n", 25), -1);
		ft_strlcpy(dst, tmp, len + 1);
	}
	else
	{
		key_len = len_keyword(&str[*i]);
		tmp = malloc((key_len + 1) * sizeof(char));
		if (!tmp || len == -1)
			return (free(tmp), write(2, "minishell: malloc failed\n", 25), -1);
		ft_strlcpy(tmp, &str[*i], key_len + 1);
		ft_strlcpy(dst, find_env_var(var->envp, tmp), len + 1);
		*i += key_len - 1;
	}
	return (free(tmp), len);
}

int	len_expanded_heredoc(t_mini *var, char *str)
{
	int	len;
	int	i;
	int	check;

	len = 0;
	i = 0;
	check = 0;
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '?'))
		{
			check = len_env_var_heredoc(var, str, &i);
			len += check;
		}
		else
			len++;
		i++;
		if (check == -1)
			return (-1);
	}
	return (len);
}

char	*cpy_expanded_heredoc(t_mini *var, char *str, char *result)
{
	int	i;
	int	j;
	int	check;

	i = 0;
	j = 0;
	check = 0;
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '?'))
		{
			check = cpy_env_var_heredoc(var, str, &i, &result[j]);
			j += check;
		}
		else
			result[j++] = str[i];
		if (check == -1)
			return (NULL);
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	*expand_heredoc(t_mini *var, char *str, t_expand *exp)
{
	int		len;
	char	*result;

	len = len_expanded_heredoc(var, str);
	if (len == -1)
	{
		free(exp);
		error_exit(var, "minishell: malloc failed\n");
	}
	result = (char *)malloc((len + 1) * sizeof(char));
	if (!result)
	{
		free(exp);
		error_exit(var, "minishell: malloc failed\n");
	}
	result = cpy_expanded_heredoc(var, str, result);
	return (result);
}
