/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:00:52 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/13 13:53:39 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal = 0;

/**
 * 
 */

int main(int argc, char **argv, char **envp)
{
	t_mini *var;

	// extra edge case? would just about fit
	// if (envp == NULL || *envp == NULL)
	// 	return (printsdr(BOLD RED "No environment found. \nExiting..", RESET));
	var = malloc(sizeof(t_mini));
	initialize_minishell(var, argc, argv, envp);
	signals_setup();
	// REPL
	// = Read > Evaluate > Print > Loop
	while (1)
	{
		g_signal = 0;
		var->line = readline(BOLD GRAY "$ " RESET);
		// making the BOLD GRAY the defult color??
		if (var->line == NULL)
		// Ctrl+D has been pressed to terminate the program
		{
			ft_printf(BOLD GRAY "Exiting..\n" RESET);
			free_var_exit(var, 0);
		}
		if (*var->line)
		{
			add_history(var->line);
			handle_command(var);
		}
		free_one_line(var);
	}
	free_var_exit(var, 0);
	return (0);
}

// envv (also called envp) = environment variables
// array of strings (character pointers), each representing one
// environment variable in the form "KEY=value", ending with a null pointer.

// readline leaks: to suppress readline leaks run:
// valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell

// ------------------ Week-by-Week Breakdown ------------------
// Week 1: Foundation
// 	Project setup, basic main loop, signal handling framework
// 	Libft integration and Makefile creation
// 	Basic readline integration and prompt display
// --------DONE--------

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