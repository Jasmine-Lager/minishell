/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_define.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:35:51 by jasminelage       #+#    #+#             */
/*   Updated: 2025/09/18 13:42:16 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to handle quote processing
int process_quote(t_mini *var, int i, char quote)
{
	i++;
	while (var->line[i] && var->line[i] != quote)
		i++;
	if (!var->line[i])
	{
		write(2, "Error: unclosed quotes\n", 23);
		return (-1);
	}
	return (i + 1);
}

// Find the end of a regular token, respecting quote boundaries
int find_token_end(t_mini *var, int start)
{
	int i;

	i = start;
	while (var->line[i])
	{
		if (var->line[i] == ' ' || var->line[i] == '|' || var->line[i] == '<' 
				|| var->line[i] == '>')
			break;
		if (var->line[i] == '"' || var->line[i] == '\'')
		{
			i = process_quote(var, i, var->line[i]);
			if (i == -1)
				return (-1);
		}
		else
			i++;
	}
	return (i);
}

// Get the end position of a metacharacter token (|, <, <<, >, >>)
int get_metachar_end(t_mini *var, int start)
{
	if (!var->line[start])
		return (0);
	if (var->line[start] == '|')
		return (start + 1);
	if (var->line[start] == '<')
	{
		if (var->line[start + 1] == '<')
			return (start + 2);
		return (start + 1);
	}
	if (var->line[start] == '>')
	{
		if (var->line[start + 1] == '>')
			return (start + 2);
		return (start + 1);
	}
	return (0); // Not a metacharacter
}

// Main tokenization function - clean separation of concerns
void define_token(t_mini *var, int *start_token, int *end_token, t_token *new)
{
	int metachar_end;

	while (var->line[*start_token] == ' ')
		(*start_token)++;
	if (!var->line[*start_token])
	{
		*end_token = *start_token;
		return;
	}
	if ((metachar_end = get_metachar_end(var, *start_token)))
	{
		*end_token = metachar_end;
		new->quotes = NONE; // Metacharacters are never quoted
		return;
	}
	*end_token = find_token_end(var, *start_token);
	if (*end_token == -1) // Error in quote handling
	{
		*end_token = *start_token; // Reset to start to indicate error
		return;
	}
	new->quotes = NONE; // Will be set later in the parsing process
}
