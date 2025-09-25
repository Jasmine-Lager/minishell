/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 11:25:20 by jasminelage       #+#    #+#             */
/*   Updated: 2025/09/25 13:24:41 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Calculate the length of the string after quote removal
int calculate_unquoted_length(char *str)
{
	int len = 0;
	int i = 0;
	char quote_char;

	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote_char = str[i];
			i++; // Skip opening quote
			while (str[i] && str[i] != quote_char)
			{
				len++;
				i++;
			}
			if (str[i] == quote_char)
				i++; // Skip closing quote
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

// Copy string content while removing quotes
void copy_without_quotes(char *dst, char *src)
{
	int i = 0;
	int j = 0;
	char quote_char;

	while (src[i])
	{
		if (src[i] == '\'' || src[i] == '"')
		{
			quote_char = src[i];
			i++; // Skip opening quote
			while (src[i] && src[i] != quote_char)
			{
				dst[j++] = src[i];
				i++;
			}
			if (src[i] == quote_char)
				i++; // Skip closing quote
		}
		else
		{
			dst[j++] = src[i];
			i++;
		}
	}
	dst[j] = '\0';
}

// Remove quotes from a token that may have multiple quoted sections
void remove_quotes_from_token(t_token *token)
{
	char *new_content;
	int new_len;

	if (!token || !token->content)
		return;
	if (!ft_strchr(token->content, '\'') && !ft_strchr(token->content, '"'))
		return;
	new_len = calculate_unquoted_length(token->content);
	new_content = malloc(new_len + 1);
	if (!new_content)
		return;
	copy_without_quotes(new_content, token->content);
	free(token->content);
	token->content = new_content;
	token->quotes = NONE; // Mark as processed
}

void remove_quotes_from_tokens(t_mini *var)
{
	t_token *current;

	current = var->tokens;
	while (current)
	{
		// Skip delimiter tokens (they should keep their quotes for heredoc detection)
		if (current->type != DELIMITER)
			remove_quotes_from_token(current);
		current = current->next;
	}
}