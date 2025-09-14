/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_to_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:53:07 by ksevciko          #+#    #+#             */
/*   Updated: 2025/09/14 17:37:37 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_first_token(t_mini *var, int *start_token, int *end_token)
{
	var->tokens = malloc(sizeof(t_token));
	if (!var->tokens)
		error_exit(var, "Malloc failled: create_first_token\n");
	var->tokens->next = NULL;
	define_token(var, start_token, end_token, var->tokens);
	// Check if we have a valid token
	if (*end_token <= *start_token)
	{
		free(var->tokens);
		var->tokens = NULL;
		return;
	}
	var->tokens->content = (char *)malloc((end_token - start_token + 1) * sizeof(char));
	if (!var->tokens->content)
		error_exit(var, "Malloc failled: create_first_token\n");
	ft_strlcpy(var->tokens->content, &var->line[*start_token],
			   *end_token - *start_token + 1);
	find_token_type(var, var->tokens, NULL);
	*start_token = *end_token;
}

// add another token to the end of the list
void	append_token(t_mini *var, int *start_token, int *end_token,
				  t_token **last)
{
	t_token *new;

	(*last)->next = malloc(sizeof(t_token));
	new = (*last)->next;
	if (!new)
		error_exit(var, "Malloc failled: append_token\n");
	new->next = NULL;
	define_token(var, start_token, end_token, new);
	// Check if we have a valid token
	if (*end_token <= *start_token)
	{
		(*last)->next = NULL;
		free(new);
		return;
	}
	new->content = (char *)malloc((*end_token - *start_token + 1) * sizeof(char));
	if (!new->content)
		error_exit(var, "Malloc failled: append_token\n");
	ft_strlcpy(new->content, &var->line[*start_token],
			   *end_token - *start_token + 1);
	find_token_type(var, new, *last);
	*last = new;
	*start_token = *end_token;
}

void	parse(t_mini *var)
{
	int start_token; // index of a start of a token in the line
	int end_token;	 // index one after the end of the token in the line
	t_token *last;

	start_token = 0;
	end_token = 0;
	if (!var->line || !*var->line)
		return;
	if (!ft_strncmp("exit", var->line, 5)) // only here now for testing, remove later
		free_var_exit(var, 0);
	create_first_token(var, &start_token, &end_token);
	if (!var->tokens)
		return;
	last = var->tokens;
	while (var->line[start_token])
	{
		// Skip spaces before trying to create next token
		while (var->line[start_token] == ' ')
			start_token++;
		// Check if we reached the end
		if (!var->line[start_token])
			break;
		append_token(var, &start_token, &end_token, &last);
	}
	// expand_tokens(var); // TODO, check double quote first
}
