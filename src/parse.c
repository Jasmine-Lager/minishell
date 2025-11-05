/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:53:07 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/05 14:41:47 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Simplified version for backward compatibility
t_quotes	analyze_token_quotes(char *content)
{
	t_quote_info	info;

	info = analyze_token_quotes_detailed(content);
	return (info.dominant_type);
}

// Analyze and set quote information after token content is created
void	set_token_quote_info(t_token *token)
{
	if (!token->content)
		return ;
	token->quotes = analyze_token_quotes(token->content);
}

int	create_first_token(t_mini *var, int *start_token, int *end_token)
{
	int	len;

	var->tokens = malloc(sizeof(t_token));
	if (!var->tokens)
		return (write(2, "Malloc failed: create_first_token\n", 34), 0);
	var->tokens->next = NULL;
	define_token(var, start_token, end_token, var->tokens);
	if (*end_token <= *start_token)
	{
		free(var->tokens);
		var->tokens = NULL;
		return (1);
	}
	len = *end_token - *start_token;
	var->tokens->content = malloc(len + 1);
	if (!var->tokens->content)
		return (write(2, "Malloc failed: create_first_token\n", 34), 0);
	ft_strlcpy(var->tokens->content, &var->line[*start_token], len + 1);
	set_token_quote_info(var->tokens);
	find_token_type(var, var->tokens, NULL);
	*start_token = *end_token;
	return (1);
}

int	append_token(t_mini *var, int *start_token, int *end_token,
		t_token **last)
{
	t_token	*new;
	int		len;

	(*last)->next = malloc(sizeof(t_token));
	new = (*last)->next;
	if (!new)
		return (write(2, "Malloc failed: append_token\n", 28), 0);
	new->next = NULL;
	define_token(var, start_token, end_token, new);
	if (*end_token <= *start_token)
	{
		(*last)->next = NULL;
		free(new);
		return (1);
	}
	len = *end_token - *start_token;
	new->content = malloc(len + 1);
	if (!new->content)
		return (write(2, "Malloc failed: append_token\n", 28), 0);
	ft_strlcpy(new->content, &var->line[*start_token], len + 1);
	set_token_quote_info(new);
	find_token_type(var, new, *last);
	*last = new;
	*start_token = *end_token;
	return (1);
}

bool	parse(t_mini *var)
{
	int		start_token;
	int		end_token;
	t_token	*last;

	start_token = 0;
	end_token = 0;
	if (!var->line || !*var->line)
		return (0);
	if (!create_first_token(var, &start_token, &end_token))
		return (0);
	if (!var->tokens)
		return (0);
	last = var->tokens;
	while (var->line[start_token])
	{
		while (var->line[start_token] == ' ')
			start_token++;
		if (!var->line[start_token])
			break ;
		if (!append_token(var, &start_token, &end_token, &last))
			return (0);
	}
	if (!expand_tokens(var) || !validate_syntax(var))
		return (0);
	return (1);
}
