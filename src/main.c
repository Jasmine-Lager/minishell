/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:00:52 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/04 19:50:01 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_mini	*var;

	var = malloc(sizeof(t_mini));
	if (!var)
	{
		write(2, "minishell: malloc failed\n", 25);
		exit (70);
	}
	initialize_var(var, argc, argv, envp);
	var->line = readline("$");
	while (var->line)
	{
		//handle signals
		if (*var->line)
			add_history(var->line);
		//handle_command
		free(var->line);
		var->line = readline("$");
	}
	return (0);
}

	// ------------------ Week-by-Week Breakdown ------------------
	// Week 1: Foundation
	// 	Project setup, basic main loop, signal handling framework
	// 	Libft integration and Makefile creation
	// 	Basic readline integration and prompt display

	// Week 2: Parsing Engine
	// 	Complete lexer implementation with comprehensive tokenization
	// 	Parser development with AST construction and syntax validation
	// 	Initial testing framework establishment

	// Week 3: Variable Processing
	// 	Expander implementation for all variable types
	// 	Environment management system creation
	// 	Built-in command framework development

	// Week 4: Execution Engine
	// 	Complete executor implementation with process management
	// 	All built-in commands with proper bash behavior replication
	// 	Comprehensive redirection and pipe handling

	// Week 5: Testing and Refinement
	// 	Extensive testing with edge cases and error conditions
	// 	Memory leak elimination and performance optimization
	// 	Code review and norm compliance verification
	// 	Final debugging and polishing