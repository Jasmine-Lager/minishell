/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_to_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:53:07 by ksevciko          #+#    #+#             */
/*   Updated: 2025/09/30 12:35:40 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Simplified version for backward compatibility
t_quotes analyze_token_quotes(char *content)
{
	t_quote_info info = analyze_token_quotes_detailed(content);
	return (info.dominant_type);
}

// Analyze and set quote information after token content is created
void set_token_quote_info(t_token *token)
{
	if (!token->content)
		return;
	token->quotes = analyze_token_quotes(token->content);
}

void create_first_token(t_mini *var, int *start_token, int *end_token)
{
	var->tokens = malloc(sizeof(t_token));
	if (!var->tokens)
		error_exit(var, "Malloc failed: create_first_token\n");
	var->tokens->next = NULL;
	define_token(var, start_token, end_token, var->tokens);
	if (*end_token <= *start_token)
	{
		free(var->tokens);
		var->tokens = NULL;
		return;
	}
	int len = *end_token - *start_token;
	var->tokens->content = malloc(len + 1);
	if (!var->tokens->content)
		error_exit(var, "Malloc failed: create_first_token\n");
	ft_strlcpy(var->tokens->content, &var->line[*start_token], len + 1);
	set_token_quote_info(var->tokens);
	find_token_type(var, var->tokens, NULL);
	*start_token = *end_token;
}

void append_token(t_mini *var, int *start_token, int *end_token, t_token **last)
{
	t_token *new;

	(*last)->next = malloc(sizeof(t_token));
	new = (*last)->next;
	if (!new)
		error_exit(var, "Malloc failed: append_token\n");
	new->next = NULL;
	define_token(var, start_token, end_token, new);
	if (*end_token <= *start_token)
	{
		(*last)->next = NULL;
		free(new);
		return;
	}
	int len = *end_token - *start_token;
	new->content = malloc(len + 1);
	if (!new->content)
		error_exit(var, "Malloc failed: append_token\n");
	ft_strlcpy(new->content, &var->line[*start_token], len + 1);
	set_token_quote_info(new);
	find_token_type(var, new, *last);
	*last = new;
	*start_token = *end_token;
}

void parse(t_mini *var)
{
	int start_token;
	int end_token;
	t_token *last;

	start_token = 0;
	end_token = 0;
	if (!var->line || !*var->line)
		return;
	if (!ft_strncmp("exit", var->line, 5)) // Testing only - remove later
		free_var_exit(var, 0);
	create_first_token(var, &start_token, &end_token);
	if (!var->tokens)
		return;
	last = var->tokens;
	while (var->line[start_token])
	{
		while (var->line[start_token] == ' ')
			start_token++;
		if (!var->line[start_token])
			break;
		append_token(var, &start_token, &end_token, &last);
	}
	expand_tokens(var);
	handle_heredocs(var);
}
