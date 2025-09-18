/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_define.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:35:51 by jasminelage       #+#    #+#             */
/*   Updated: 2025/09/18 12:52:16 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// Helper function to handle single quote logic
void handle_single_quote(int *in_single, t_quote_counts *counts)
{
	if (!(*in_single))
	{
		*in_single = 1;
		counts->single_sections++;
	}
	else
		*in_single = 0;
}

// Helper function to handle double quote logic
void handle_double_quote(int *in_double, t_quote_counts *counts)
{
	if (!(*in_double))
	{
		*in_double = 1;
		counts->double_sections++;
	}
	else
		*in_double = 0;
}

// Count quote sections in token content
t_quote_counts count_quote_sections(char *content)
{
	t_quote_counts counts;
	int i;
	int in_single;
	int in_double;

	counts.single_sections = 0;
	counts.double_sections = 0;
	counts.unquoted_sections = 0;
	i = 0;
	in_single = 0;
	in_double = 0;
	while (content[i])
	{
		if (content[i] == '\'' && !in_double)
			handle_single_quote(&in_single, &counts);
		else if (content[i] == '"' && !in_single)
			handle_double_quote(&in_double, &counts);
		else if (!in_single && !in_double &&
				 content[i] != '\'' && content[i] != '"')
			if (i == 0 || (content[i - 1] == '\'' || content[i - 1] == '"'))
				counts.unquoted_sections++;
		i++;
	}
	return (counts);
}

// Determine dominant quote type for mixed quotes
t_quotes get_dominant_type(t_quote_counts counts)
{
	if (counts.double_sections > counts.single_sections)
		return (DOUBLE);
	if (counts.single_sections > 0)
		return (SINGLE);
	return (NONE);
}

// Helper function to check if quotes are pure single type
int is_pure_single(t_quote_counts counts)
{
	return (counts.single_sections == 1 &&
			counts.double_sections == 0 && counts.unquoted_sections == 0);
}

// Helper function to check if quotes are pure double type
int is_pure_double(t_quote_counts counts)
{
	return (counts.double_sections == 1 &&
			counts.single_sections == 0 && counts.unquoted_sections == 0);
}

// Set quote info based on section counts
t_quote_info set_quote_behavior(t_quote_counts counts)
{
	t_quote_info info;

	info.mixed_quotes = 0;
	info.needs_processing = 0;
	info.dominant_type = NONE;
	if (is_pure_single(counts))
	{
		info.dominant_type = SINGLE;
		info.needs_processing = 1;
	}
	else if (is_pure_double(counts))
	{
		info.dominant_type = DOUBLE;
		info.needs_processing = 1;
	}
	else if (counts.single_sections > 0 || counts.double_sections > 0)
	{
		info.mixed_quotes = 1;
		info.needs_processing = 1;
		info.dominant_type = get_dominant_type(counts);
	}
	return (info);
}

t_quote_info analyze_token_quotes_detailed(char *content)
{
	t_quote_counts counts;
	t_quote_info info;

	counts = count_quote_sections(content);
	info = set_quote_behavior(counts);
	return (info);
}

// Simplified version for backward compatibility
t_quotes analyze_token_quotes(char *content)
{
	t_quote_info info = analyze_token_quotes_detailed(content);
	return (info.dominant_type);
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
