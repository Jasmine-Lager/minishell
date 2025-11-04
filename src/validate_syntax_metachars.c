/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax_metachars.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 13:31:28 by jasminelage       #+#    #+#             */
/*   Updated: 2025/11/04 15:45:47 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_syntax_error(char *token)
{
	write(2, "minishell: syntax error near unexpected token `", 48);
	if (token)
		write(2, token, ft_strlen(token));
	else
		write(2, "newline", 7);
	write(2, "'\n", 2);
}

static bool	is_redir_operator(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC);
}

static bool	check_redir_token(t_token *token, t_mini *var)
{
	if (!token->next || is_redir_operator(token->next->type)
		|| token->next->type == PIPE)
	{
		if (!token->next || is_redir_operator(token->next->type))
			print_syntax_error(NULL);
		else
			print_syntax_error(token->next->content);
		var->exit_code = 2;
		return (false);
	}
	return (true);
}

static bool	check_pipe_token(t_token *token, t_mini *var)
{
	if (!token->next)
	{
		// Trailing pipe - set continuation flag
		var->needs_continuation = 1;
		return (true);  // Don't error, allow continuation
	}
	if (token->next->type == PIPE)
	{
		print_syntax_error(token->next->content);
		var->exit_code = 2;
		return (false);
	}
	return (true);
}


bool	validate_syntax(t_mini *var)
{
t_token	*token;

	token = var->tokens;
	if (!token)
		return (true);
	if (token->type == PIPE)
	{
		print_syntax_error(token->content);
		var->exit_code = 2;
		return (false);
	}
	while (token)
	{
		if (is_redir_operator(token->type) && !check_redir_token(token, var))
			return (false);
		if (token->type == PIPE && !check_pipe_token(token, var))
			return (false);
		token = token->next;
	}
	return (true);
}
