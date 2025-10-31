/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:00:52 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/31 19:20:19 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	t_mini	*var;

	var = malloc(sizeof(t_mini));
	initialize_minishell(var, argc, argv, envp);
	signals_setup();
	var->line = readline("$ ");
	while (var->line)
	{
		if (g_signal == 130) // Ctrl+C was pressed
		{
			var->exit_code = 130;
			g_signal = 0;
		}
		if (*var->line)
			handle_command(var);
		free_one_line(var);
		var->line = readline("$ ");
	}
	write(2, "Exiting..\n", 10);
	free_var_exit(var, 0);
	return (0);
}

// envv (also called envp) = environment variables
// array of strings (character pointers), each representing one
// environment variable in the form "KEY=value", ending with a null pointer.

// readline leaks: to suppress readline leaks run:
// valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell
