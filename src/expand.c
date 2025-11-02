/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:57 by jlager            #+#    #+#             */
/*   Updated: 2025/11/02 15:06:32 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int mark_where_to_split(t_expand *exp, char *result, int j)
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

void	expand_str(t_mini *var, char *str, t_expand *exp)
{
	exp->dquote = 0;
	exp->squote = 0;
	exp->len = 0;
	if (!len_expanded(var, str, exp))
	{
		write(2, "minishell: malloc failed\n", 25);
		return ;
	}
	exp->result = (char *)malloc((exp->len + 1) * sizeof(char));
	exp->i_start_split = (int *)malloc(exp->nbr_split * sizeof(int));
	exp->i_end_split = (int *)malloc(exp->nbr_split * sizeof(int));
	if (!exp->result || !exp->i_start_split || !exp->i_end_split)
	{
		free(exp->result); //the others must be freed in save_result_in_token
		exp->result = NULL;
		write(2, "minishell: malloc failed\n", 25);
		return ;
	}
	exp->dquote = 0;
	exp->squote = 0;
	exp->i = 0;
	cpy_expanded(var, str, exp->result, exp);
	var->cmd = NULL;
}

int	add_new_token(t_mini *var, t_expand *exp, int start, int end)
{
	t_token *new;

	new = malloc(sizeof(t_token));
	if (!new)
	{
		exp->check = 0;
		return (free(exp->result), 1);
	}
	new->content = (char *)malloc((end - start + 1) * sizeof(int));
}

int	split_words(t_mini *var, t_expand *exp)
{

	//handle first chunk if there are no spaces at the start
	exp->i = 0;
	while (exp->i < exp->nbr_split)// +-1???// && exp->i_end_split[exp->i] != exp->len)
	{
		add_new_token(var, exp, exp->i_end_split[exp->i], exp->i_start_split[exp->i + 1])
	}
	if (exp->i_end_split[exp->i] != exp->len)
		//handle last chunk
}

int	save_result_in_token(t_mini *var, t_expand *exp)
{
	exp->check = 1;
	if (!(exp->result))
	{
		write(2, "minishell: malloc failed\n", 25); //check all error messages, i think that insome cases this gets printed twice
		free(exp->i_start_split);
		free(exp->i_end_split);
		free(exp);
		return (0);
	}
	free(exp->current->content);
	if (!*(exp->result) && exp->can_be_rm)
		empty_token(var, exp->last, &(exp->current), exp->result);
	else if (!split_words(var, exp))
	{
		exp->current->content = exp->result;
		exp->current = exp->current->next;
	}
	exp->result = NULL;
	free(exp->i_start_split);
	exp->i_start_split = NULL;
	free(exp->i_end_split);
	exp->i_end_split = NULL;
	return (exp->check);
}

bool	expand_tokens(t_mini *var)
{
	t_expand	*exp;

	exp = init_expanded(var->tokens);
	if (!exp)
		return (0);
	while (exp->current)
	{
		exp->can_be_rm = 1;
		exp->nbr_split = 0;
		if (exp->current->type == DELIMITER)
		{
			if (!heredoc(var, exp->current))
				return (free(exp), 0); //free exp properly first!!!!!!
			exp->current = exp->current->next;
			continue ;
		}
		expand_str(var, exp->current->content, exp);
		if (!save_result_in_token(var, exp))
			return (0);
	}
	free(exp);
	return (1);
}
