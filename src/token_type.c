/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:30 by jlager            #+#    #+#             */
/*   Updated: 2025/11/02 15:46:54 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		new->type = REDIR_APPEND;
	}
	else if (ft_strncmp(new->content, "<<", 3) == 0)
	{
		new->type = HEREDOC;
	}
	else
		out = 0;
	return (out);
}

bool	check_in_out_delim(t_token *new, t_token *last)
{
	bool	out;

	out = 1;
	if (last && last->type == HEREDOC)
	{
		new->type = DELIMITER;
	}
	else if (last && last->type == REDIR_IN)
	{
		new->type = INFILE;
	}
	else if (last && (last->type == REDIR_OUT || last->type == REDIR_APPEND))
	{
		new->type = OUTFILE;
	}
	else
		out = 0;
	return (out);
}

char	first_non_quote_char(char *str)
{
	while (*str == '"' || *str == 39)
	{
		str++;
	}
	return (*str);
}

void	check_cmd_flag_word(t_token *new, t_token *last)
{
	if (last && (last->type == CMD || last->type == FLAG
			|| last->type == WORD || last->type == INFILE
			|| last->type == OUTFILE || last->type == DELIMITER)
		&& first_non_quote_char(new->content) == '-')
		new->type = FLAG;
	else if ((last && (last->type == PIPE || last->type == INFILE
				|| last->type == OUTFILE || last->type == DELIMITER)) || !last)
		new->type = CMD;
	else
		new->type = WORD;
}

void	find_token_type(t_mini *var, t_token *new, t_token *last)
{
	if (check_metacharacters(var, new))
		;
	else if (check_in_out_delim(new, last))
		;
	else
	{
		check_cmd_flag_word(new, last);
	}
}
