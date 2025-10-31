/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_define.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:35:51 by jasminelage       #+#    #+#             */
/*   Updated: 2025/10/31 17:02:20 by jlager           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to skip past a quoted section
int	skip_quoted_section(t_mini *var, int i, char quote)
{
	i++;
	while (var->line[i] && var->line[i] != quote)
		i++;
	if (var->line[i] != quote)
	{
		write(2, "error: unclosed quotes\n", 23);
		return (i);
	}
	i++;
	return (i);
}

// Check if character at position i is a token boundary
int	is_token_boundary(t_mini *var, int i)
{
	if (!var->line[i])
		return (1);
	if (var->line[i] == ' ' || var->line[i] == '\t')
		return (1);
	if (var->line[i] == '|' || var->line[i] == '<' || var->line[i] == '>')
		return (1);
	return (0);
}

// Find the end of a token that may contain multiple quoted/unquoted sections
int	find_complete_token_end(t_mini *var, int start)
{
	int	i;

	i = start;
	while (var->line[i] && !is_token_boundary(var, i))
	{
		if (var->line[i] == '"')
			i = skip_quoted_section(var, i, '"');
		else if (var->line[i] == '\'')
			i = skip_quoted_section(var, i, '\'');
		else
			i++;
	}
	return (i);
}

// Get the end position of a metacharacter token (|, <, <<, >, >>)
int	get_metachar_end(t_mini *var, int start)
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
	return (0);
}

// Main tokenization function
void	define_token(t_mini *var, int *start_token, int *end_token,
		t_token *new)
{
	int	metachar_end;

	while (var->line[*start_token] == ' ' || var->line[*start_token] == '\t')
		(*start_token)++;
	if (!var->line[*start_token])
	{
		*end_token = *start_token;
		return ;
	}
	if (metachar_end = get_metachar_end(var, *start_token))
	{
		*end_token = metachar_end;
		new->quotes = NONE;
		return ;
	}
	*end_token = find_complete_token_end(var, *start_token);
	new->quotes = NONE;
}
