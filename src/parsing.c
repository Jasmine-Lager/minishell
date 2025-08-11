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

void	find_start_end_of_token(t_mini *var, int *start_token, int *end_token,
		t_token *new) //needs to be modified a lot - should make |, <, >, <<, >> in a single token regardless of whether there are spaces around, also this logic for end_token does not work.
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

void	create_first_token(t_mini *var, int *start_token, int *end_token)
{
	var->tokens = malloc(sizeof(t_token));
	if (!var->tokens)
		other_error(var, "malloc failled\n");
	var->tokens->next = NULL;
	find_start_end_of_token(var, start_token, end_token, var->tokens);
	var->tokens->content = (char *)malloc((end_token - start_token + 1)
			* sizeof(char));
	if (!var->tokens->content)
		other_error(var, "malloc failled\n");
	ft_strlcpy(var->tokens->content, &var->line[*start_token],
		end_token - start_token + 1);
	find_token_type(var, var->tokens, NULL);
	*start_token = *end_token;
}

void	create_one_token(t_mini *var, int *start_token, int *end_token,
			t_token **last)
{
	t_token	*new;

	(*last)->next = malloc(sizeof(t_token));
	new = (*last)->next;
	if (!new)
		other_error(var, "malloc failled\n");
	new->next = NULL;
	find_start_end_of_token(var, start_token, end_token, new);
	new->content = (char *)malloc((*end_token - *start_token + 1)
			* sizeof(char));
	if (!new->content)
		other_error(var, "malloc failled\n");
	ft_strlcpy(new->content, &var->line[*start_token],
		*end_token - *start_token + 1);
	find_token_type(var, new, *last);
	*last = new;
	*start_token = *end_token;
}

void	parse(t_mini *var)
{
	int			start_token; //index of a start of a token in the line
	int			end_token; //index one after the end of the token in the line
	t_token		*last;

	start_token = 0;
	end_token = 0;
	if (!ft_strncmp("exit", var->line, 5)) //only here now for testing, remove later
		free_var_exit(var, 0);
	create_first_token(var, &start_token, &end_token);
	last = var->tokens;
	while (var->line[start_token])
		create_one_token(var, &start_token, &end_token, &last);
	//expand(var); //expand $ here, check if the token was quoted first
}

// The parser validates syntax and constructs an Abstract Syntax Tree (AST) 
// representing command structure. It ensures proper operator placement and
// quote matching.

//!!DO NOT expand $ for the delimiter

// Key Responsibilities:
// Syntax validation (no consecutive operators, balanced quotes)
// AST construction for execution planning
// Error detection and reporting
// Command grouping for pipes and redirections
