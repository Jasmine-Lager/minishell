/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:00:52 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/05 16:24:46 by jlager           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_mini	*var; // this I would maybe call input or something
		// like t_minishell *input

	var = malloc(sizeof(t_mini));
	if (!var)
	// return_error("malloc in main.c failed"); one line only and more readable?
	{
		write(2, "minishell: malloc failed\n", 25);
		exit(70);
	}
	if (!initialize_var(var, argc, argv, envp)); // rename to initalize_minishell or bootup_minishell for readability?
		return_error("Initialization failed");
	setup_signals();
	;
	// REPL
	// = Read > Evaluate > Print > Loop
	while (var->line)
	{
		var->line = readline("$ ");
		if (var->line == NULL) // Ctrl+D has been pressed to terminate the program
		{
			ft_printf("exiting..\n");
			break ; // terminating program
		}	
		if (*var->line)
		{
			add_history(var->line);
			// handle_command(var);
		}
		free(var->line);
	}
	free_var_exit(var, 0);
	return (0);
}

// envv (also called envp) = environment variables
// array of strings (character pointers), each representing one
// environment variable in the form "KEY=value", ending with a null pointer.

// t_prompt	prompt;

// 	// 1. Initialize shell and env structures to "boot up"
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