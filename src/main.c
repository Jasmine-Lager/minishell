/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:00:52 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/05 14:57:05 by ksevciko         ###   ########.fr       */
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
		//handle signals()
		if (*var->line)
			add_history(var->line);
		handle_command(var);
		free(var->line);
		var->line = readline("$");
	}
	free_var_exit(var, 0);
	return (0);
}

// envv (also called envp) = environment variables
// array of strings (character pointers), each representing one
// environment variable in the form "KEY=value", ending with a null pointer.

//readline leaks: to suppress readline leaks run: 
// valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell

// t_prompt	prompt;

// 	// 1. Initialize shell and env structures
// 	while ()
// 	{
// 		// 2. Display prompt and read line
// 		// 3. Parse input (quotes, syntax, split, redir)
// 		// 4. Expand environment variables  (for variables, etc.)
// 		// 5. Execute command(s) (handle pipes, redirs, built-ins, external)
// 		// 6. Cleanup (to avoid leaks)
// 	}
// 	// Free resources and exit

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