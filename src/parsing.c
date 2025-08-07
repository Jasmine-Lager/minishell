/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:53:07 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/07 14:45:38 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_start_end_of_token(t_mini *var, int *start_token, int *end_token, t_token *new) //needs to be modified a lot - should make |, <, >, <<, >> in a single token regardless of whether there are spaces around, also this logic for end_token does not work.
{
	char	divider;

	while (var->line[*start_token] == ' ')
		(*start_token)++;
	new->quotes = NONE;
	if (var->line[*start_token] == '"')
	{
		divider = '"';
		new->quotes = DOUBLE;
		(*start_token)++;
	}
	else if (var->line[*start_token] == 39)
	{
		divider = 39;
		new->quotes = SINGLE;
		(*start_token)++;
	}
	else
		divider = ' ';
	*end_token = *start_token + 1;
	while (var->line[*end_token] && var->line[*end_token] != divider)
		(*end_token)++;
	if (var->line[*end_token] == '\0' && divider != ' ')
		other_error(var, "error: unclosed quotes\n");
}

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
		var->append_mode = 1;
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
	else if (last && (last->type == CMD || last->type == FLAG) &&
		*new->content == '-')
		new->type = FLAG;
	else if ((last && (last->type == PIPE || last->type == INFILE
		|| last->type == OUTFILE || last->type == DELIMITER)) || !last)
		new->type = CMD;
	else
		new->type = WORD;
}

void	create_one_token(t_mini *var, int *start_token, int *end_token,
			t_token **last)
{
	t_token	*new;

	if (!var->tokens || !*last)
	{
		var->tokens = malloc(sizeof(t_token));
		new = var->tokens;
	}
	else
	{
		(*last)->next = malloc(sizeof(t_token));
		new = (*last)->next;
	}
	if (!new)
		other_error(var, "malloc failled\n");
	new->next = NULL;
	find_start_end_of_token(var, start_token, end_token, new);
	new->content = (char *)malloc((end_token - start_token + 1)
		* sizeof(char));
	if (!new->content)
		other_error(var, "malloc failled\n");
	ft_strlcpy(new->content, &var->line[*start_token],
		end_token - start_token + 1);
	find_token_type(var, new, *last);
	*last = new;
	*start_token = *end_token;
}

void	parse(t_mini *var)
{
	int			start_token; //index of a start of a token in the line
	int			end_token; //index one after the end of the token in the line
	t_token		**last;

	start_token = 0;
	end_token = 0;
	last = malloc(sizeof(t_token *));
	if (!last)
		other_error(var, "malloc failed\n");
	*last = NULL;
	while (var->line[start_token])
		create_one_token(var, &start_token, &end_token, last);

	
	// if (!ft_strncmp("exit", var->line, 5))
	// 	free_var_exit(var, 0);
}

// The parser validates syntax and constructs an Abstract Syntax Tree (AST) 
// representing command structure. It ensures proper operator placement and quote matching.

//!!DO NOT expand $ for the delimiter

// Key Responsibilities:
// Syntax validation (no consecutive operators, balanced quotes)
// AST construction for execution planning
// Error detection and reporting
// Command grouping for pipes and redirections
