/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 03:16:58 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/02 03:16:58 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
