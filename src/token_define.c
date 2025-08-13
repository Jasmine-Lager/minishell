/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_define.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:35:51 by jasminelage       #+#    #+#             */
/*   Updated: 2025/08/13 17:43:39 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		write(2, "Error: unclosed quotes\n", 23);
	else
		(*end_token)++;
	return (1); // handled
}

// Detect metachar tokens |, <, >, <<, >>
int get_metachar_end(t_mini *var, int start)
{
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

// Set the end_token index to mark token boundary, starting from *start_token
void define_token(t_mini *var, int *start_token, int *end_token, t_token *new)
{
	char divider;
	int metachar_end;

	while (var->line[*start_token] == ' ')
		(*start_token)++;
	new->quotes = NONE;
	divider = get_divider(var, *start_token, new);
	if (divider == '"' || divider == 39)
		if (quoted_token(var, start_token, end_token, divider))
			return;
	if ((metachar_end = get_metachar_end(var, *start_token)))
	{
		*end_token = metachar_end;
		return;
	}
	*end_token = *start_token;
	while (var->line[*end_token] != ' ' && !get_metachar_end(var, *end_token))
		(*end_token)++;
}
