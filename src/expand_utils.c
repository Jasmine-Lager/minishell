/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 03:16:58 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/03 21:37:09 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mark_where_to_split(t_expand *exp, char *result, int j)
{
	int	in_word;
	int	i;

	if (exp->dquote)
		return (1);
	if (exp->check == -1)
		return (0);
	in_word = 1;
	i = 0;
	while (i <= exp->check && exp->i < exp->nbr_split)
	{
		if (in_word && my_isspace(result[j + i]))
		{
			in_word = 0;
			exp->i_start_split[exp->i] = j + i;
		}
		else if (!in_word && !my_isspace(result[j + i]))
		{
			in_word = 1;
			exp->i_end_split[exp->i] = j + i;
			exp->i++;
		}
		i++;
	}
	return (1);
}

char	*cpy_expanded(t_mini *var, char *str, char *result, t_expand *exp)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (str[++i])
	{
		if (str[i] == '"' && !exp->squote)
			exp->dquote = 1 - exp->dquote;
		else if (str[i] == 39 && !exp->dquote)
			exp->squote = 1 - exp->squote;
		else if (str[i] == '$' && !exp->squote
			&& (ft_isalnum(str[i + 1]) || str[i + 1] == '?') && (++i))
		{
			exp->check = cpy_env_var(var, str, &i, &result[j]);
			if (!mark_where_to_split(exp, result, j))
				return (NULL);
			j += exp->check;
		}
		else
			result[j++] = str[i];
	}
	result[j] = '\0';
	return (result);
}

int	cpy_env_var(t_mini *var, char *str, int *i, char *dst)
{
	char	*tmp;
	int		nbr;

	nbr = 0;
	if (str[*i] == '?')
	{
		tmp = ft_itoa(var->exit_code);
		if (!tmp)
			return (write(2, "minishell: malloc failed\n", 25), -1);
		nbr = ft_strlen(tmp);
		ft_strlcpy(dst, tmp, nbr + 1);
	}
	else
	{
		nbr = len_keyword(&str[*i]);
		tmp = malloc((nbr + 1) * sizeof(char));
		if (!tmp)
			return (write(2, "minishell: malloc failed\n", 25), -1);
		ft_strlcpy(tmp, &str[*i], nbr + 1);
		*i += nbr - 1;
		var->cmd = find_env_var(var->envp, tmp);
		nbr = ft_strlen(var->cmd);
		ft_strlcpy(dst, var->cmd, nbr + 1);
	}
	return (free(tmp), nbr);
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

t_expand	*init_expanded(t_token *first_token)
{
	t_expand	*exp;

	exp = malloc(sizeof(t_expand));
	if (!exp)
		return (NULL);
	exp->result = NULL;
	exp->current = first_token;
	exp->last = NULL;
	exp->i_start_split = NULL;
	exp->i_end_split = NULL;
	exp->fd = 1;
	return (exp);
}
