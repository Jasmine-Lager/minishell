/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 11:25:20 by jasminelage       #+#    #+#             */
/*   Updated: 2025/09/18 13:07:12 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Remove quotes after expansion (separate function for clarity)
void remove_quotes_from_token(t_token *token)
{
	char *new_content;
	int len;

	if (!token || !token->content || token->quotes == NONE)
		return;
	len = ft_strlen(token->content);
	// For quoted tokens, remove the outer quotes
	if ((token->quotes == SINGLE || token->quotes == DOUBLE) && len >= 2)
	{
		new_content = ft_substr(token->content, 1, len - 2);
		if (new_content)
		{
			free(token->content);
			token->content = new_content;
			token->quotes = NONE; // Mark as processed
		}
	}
}

void remove_quotes_from_tokens(t_mini *var)
{
	t_token *current;

	current = var->tokens;
	while (current)
	{
		remove_quotes_from_token(current);
		current = current->next;
	}
}