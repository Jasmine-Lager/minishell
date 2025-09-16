/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:30 by jlager            #+#    #+#             */
/*   Updated: 2025/09/16 12:11:54 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// The lexer breaks input strings into meaningful tokens while handling quote
// parsing and special characters. This stage transforms raw input like
// echo "hello world" | wc-l > output.txt into discrete tokens.

// COMMAND, ARGUMENT, PIPE, REDIRECT_IN, REDIRECT_OUT, etc.
// Handle single and double quotes appropriately
// Manage whitespace and special character separation
// Store tokens in a linked list or array structure

bool	check_metacharacters(t_mini *var, t_token *new)
{
	bool	out;

	out = 1;
	if (ft_strncmp(new->content, "|", 2) == 0)
	{
		new->type = PIPE;
		var->nbr_pipes++;
	}
	else if (ft_strncmp(new->content, "<", 2) == 0)
		new->type = REDIR_IN;
	else if (ft_strncmp(new->content, ">", 2) == 0)
		new->type = REDIR_OUT;
	else if (ft_strncmp(new->content, ">>", 3) == 0)
	{
		new->type = REDIR_APPEND; //what should happen if there are multiple redirs?
		var->append_mode = 1; // mmm what does bash do?
	}
	else if (ft_strncmp(new->content, "<<", 3) == 0)
	{
		new->type = HEREDOC;
		var->here_doc = 1;
	}
	else
		out = 0;
	return (out);
}

bool	check_in_out_delim(t_mini *var, t_token *new, t_token *last)
{
	bool	out;

	out = 1;
	if (last && last->type == HEREDOC)
	{
		new->type = DELIMITER;
		var->delimiter = new->content;
		if 	(new->quotes == SINGLE || new->quotes == DOUBLE)
			var->delim_quoted = 1;
		var->nbr_pipes++; //heredoc needs a pipe too
	}
	else if (last && last->type == REDIR_IN)
	{
		new->type = INFILE;
		var->infile = new->content;
	}
	else if (last && (last->type == REDIR_OUT || last->type == REDIR_APPEND))
	{
		new->type = OUTFILE;
		var->outfile = new->content;
	}
	else
		out = 0;
	return (out);
}

void	find_token_type(t_mini *var, t_token *new, t_token *last)
{
	if (check_metacharacters(var, new))
		;
	else if (check_in_out_delim(var, new, last))
		;
	else if (last && (last->type == CMD || last->type == FLAG)
		&& *new->content == '-')
		new->type = FLAG;
	else if ((last && (last->type == PIPE || last->type == INFILE
				|| last->type == OUTFILE || last->type == DELIMITER)) || !last)
		new->type = CMD;
	else
		new->type = WORD;
}
