/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:57 by jlager            #+#    #+#             */
/*   Updated: 2025/11/02 01:23:11 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// DO NOT expand anything in delimiter, do not do variable 
// substitution for delimiter
// DO mark if each token was quoted

#include "minishell.h"

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
			// mark_where_to_split(exp, &result[j], exp->check);
			j += exp->check;
		}
		else
			result[j++] = str[i];
		if (exp->check == -1)
			return (NULL);
	}
	result[j] = '\0';
	return (result);
}

void	expand_str(t_mini *var, char *str, t_expand *exp)
{
	exp->dquote = 0;
	exp->squote = 0;
	exp->len = 0;
	exp->check = 0;
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
	cpy_expanded(var, str, exp->result, exp);
	var->cmd = NULL;
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
	return (exp);
}

int	save_result_in_token(t_mini *var, t_expand *exp)
{
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
	else
	{
		// if (!split_words())
		// 	return (0); //free exp first
		exp->current->content = exp->result;
		exp->current = exp->current->next;
	}
	exp->result = NULL;
	free(exp->i_start_split);
	exp->i_start_split = NULL;
	free(exp->i_end_split);
	exp->i_end_split = NULL;
	return (1);
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
