/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:00:52 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/07 15:25:43 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_mini	*var; // this I would maybe call input or something -but it is not just input, but all variables needed by the whole program (like pipes)
		// like t_minishell *input - this is very long, and would make it harder to meet norminette rules, if this name would be everywhere in the project
		//could be t_shell *info, or t_mini *struct maybe? i like t_mini *var though, and think it would be waste of time to rename it. do it if you think it would improve the project though, but change it to something short and change it everywhere

	var = malloc(sizeof(t_mini));
	initialize_minishell(var, argc, argv, envp); //no need to check return value, it does not return, on error it exits
	// setup_signals();
	// ;
	// REPL
	// = Read > Evaluate > Print > Loop
	while (1)
	{
		var->line = readline("$ ");
		if (var->line == NULL) // Ctrl+D has been pressed to terminate the program
		{
			ft_printf("exiting..\n");
			free_var_exit(var, 0);
		}	
		if (*var->line)
		{
			add_history(var->line);
			handle_command(var);
		}
		free(var->line);
		var->line = NULL; //needs to be here so free_var_exit does not do double free if the line is empty
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