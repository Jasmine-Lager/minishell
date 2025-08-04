/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:53:07 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/04 19:53:18 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// The parser validates syntax and constructs an Abstract Syntax Tree (AST) 
// representing command structure. It ensures proper operator placement and quote matching.

// Key Responsibilities:
// Syntax validation (no consecutive operators, balanced quotes)
// AST construction for execution planning
// Error detection and reporting
// Command grouping for pipes and redirections
