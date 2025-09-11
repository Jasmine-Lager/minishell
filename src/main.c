/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:00:52 by ksevciko          #+#    #+#             */
/*   Updated: 2025/09/11 16:48:04 by jasminelage      ###   ########.fr       */
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