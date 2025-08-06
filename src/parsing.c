/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:53:07 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/05 14:11:49 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse(t_mini *var)
{
	if (!ft_strncmp("exit", var->line, 5))
		free_var_exit(var, 0);
	// var->tokens = malloc(sizeof(t_token));
	// var->tokens->content = var->line;
}

// The parser validates syntax and constructs an Abstract Syntax Tree (AST) 
// representing command structure. It ensures proper operator placement and quote matching.

//!!DO NOT expand $ for the delimiter

// Key Responsibilities:
// Syntax validation (no consecutive operators, balanced quotes)
// AST construction for execution planning
// Error detection and reporting
// Command grouping for pipes and redirections
