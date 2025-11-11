/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:57 by jlager            #+#    #+#             */
/*   Updated: 2025/11/11 13:20:08 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_str(t_mini *var, char *str, t_expand *exp)
{
	exp->dquote = 0;
	exp->squote = 0;
	exp->len = 0;
	exp->result = NULL;
	if (!len_expanded(var, str, exp))
		return ;
	exp->result = (char *)malloc((exp->len + 1) * sizeof(char));
	exp->i_start_split = (int *)malloc(exp->nbr_split * sizeof(int));
	exp->i_end_split = (int *)malloc(exp->nbr_split * sizeof(int));
	if (!exp->result || !exp->i_start_split || !exp->i_end_split)
	{
		free(exp->result);
		exp->result = NULL;
		return ;
	}
	exp->dquote = 0;
	exp->squote = 0;
	exp->i = 0;
	cpy_expanded(var, str, exp->result, exp);
	var->cmd = NULL;
}

int	add_new_token(t_mini *var, t_expand *exp, int from, int to)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
	{
		exp->check = 0;
		return (free(exp->result), 0);
	}
	new->content = (char *)malloc((to - from + 1) * sizeof(char));
	if (!new->content)
	{
		exp->check = 0;
		return (free(exp->result), free(new), 0);
	}
	ft_strlcpy(new->content, &(exp->result[from]), to - from + 1);
	if (!check_in_out_delim(new, exp->last))
		check_cmd_flag_word(new, exp->last);
	new->next = exp->current->next;
	if (!exp->last)
		var->tokens = new;
	else
		exp->last->next = new;
	exp->last = new;
	return (1);
}

int	split_words(t_mini *var, t_expand *exp)
{
	if (exp->nbr_split == 0)
		return (0);
	if (exp->i_start_split[0] != 0)
	{
		if (!add_new_token(var, exp, 0, exp->i_start_split[0]))
			return (free(exp->result), 1);
	}
	exp->i = 0;
	while (exp->i < exp->nbr_split - 1)
	{
		if (!add_new_token(var, exp, exp->i_end_split[exp->i],
				exp->i_start_split[exp->i + 1]))
			return (free(exp->result), 1);
		exp->i++;
	}
	if (exp->i_end_split[exp->i] != exp->len)
	{
		if (!add_new_token(var, exp, exp->i_end_split[exp->i], exp->len))
			return (free(exp->result), 1);
	}
	free(exp->current);
	exp->current = exp->last->next;
	free(exp->result);
	return (1);
}

int	save_result_in_token(t_mini *var, t_expand *exp)
{
	exp->check = 1;
	if (!(exp->result))
	{
		write(2, "minishell: malloc in expand_str failed\n", 25);
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
		exp->last = exp->current;
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
			if (!heredoc(var, exp->current, exp))
				return (free(exp), 0);
			exp->current = exp->current->next;
			continue ;
		}
		expand_str(var, exp->current->content, exp);
		if (!save_result_in_token(var, exp))
		{
			write(2, "minishell: malloc in add_new_token failed\n", 42);
			return (0);
		}
	}
	free(exp);
	return (1);
}
