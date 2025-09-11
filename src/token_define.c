/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_define.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:35:51 by jasminelage       #+#    #+#             */
/*   Updated: 2025/09/11 16:41:02 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_unclosed_quotes(t_mini *var, int start, int end)
{
	int i;

	i = start;
	while (i < end)
	{
		if (var->line[i] == '"' || var->line[i] == 39)
		{
			char quote = var->line[i];
			i++; // Skip opening quote
			// Look for closing quote
			while (i < end && var->line[i] != quote)
				i++;
			if (i >= end) // Reached end without finding closing quote
			{
				write(2, "Error: unclosed quotes\n", 23);
				return (1); // Found unclosed quote
			}
			i++; // Skip closing quote
		}
		else
			i++;
	}
	return (0); // No unclosed quotes found
}

// Return the divider character to determine token boundaries and set quote type
char get_divider(t_mini *var, int start, t_token *new)
{
	if (var->line[start] == '"')
	{
		new->quotes = DOUBLE;
		return ('"');
	}
	else if (var->line[start] == 39) // single quote '
	{
		new->quotes = SINGLE;
		return (39);
	}
	new->quotes = NONE;
	return (' ');
}

int quoted_token(t_mini *var, int *start_token, int *end_token, char divider)
{
	(*start_token)++;
	*end_token = *start_token;
	while (var->line[*end_token] && var->line[*end_token] != divider)
		(*end_token)++;
	if (!var->line[*end_token])
	{
		write(2, "Error: unclosed quotes\n", 23);
		return (1);
	}
	else
		(*end_token)++;
	return (1); // handled
}

// Detect metachar tokens |, <, >, <<, >>
int get_metachar_end(t_mini *var, int start)
{
	if (!var->line[start]) // Add null check
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

// Helper function to find the end of a mixed token (text with quotes)
int find_mixed_token_end(t_mini *var, int start)
{
	int i = start;

	while (var->line[i] && var->line[i] != ' ' && !get_metachar_end(var, i))
	{
		if (var->line[i] == '"' || var->line[i] == 39)
		{
			char quote = var->line[i];
			i++; // Skip opening quote
			// Find closing quote
			while (var->line[i] && var->line[i] != quote)
				i++;
			if (!var->line[i]) // Unclosed quote
				return (i);
			i++; // Skip closing quote
		}
		else
			i++;
	}
	return (i);
}

// Set the end_token index to mark token boundary, starting from *start_token
void define_token(t_mini *var, int *start_token, int *end_token, t_token *new)
{
	char divider;
	int metachar_end;

	while (var->line[*start_token] == ' ')
		(*start_token)++;
	if (!var->line[*start_token])
	{
		*end_token = *start_token;
		return;
	}
	new->quotes = NONE;
	if ((metachar_end = get_metachar_end(var, *start_token)))
	{
		*end_token = metachar_end;
		return;
	}
	if (var->line[*start_token] == '"' || var->line[*start_token] == 39)
	{
		divider = get_divider(var, *start_token, new);
		if (quoted_token(var, start_token, end_token, divider))
			return;
	}
	*end_token = find_mixed_token_end(var, *start_token);
	if (check_unclosed_quotes(var, *start_token, *end_token))
		*end_token = *start_token;
}
