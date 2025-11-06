/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:00:52 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/06 16:12:47 by jasminelage      ###   ########.fr       */
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
		if (g_signal == 130)
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

// readline leaks: to suppress readline leaks run:
// valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all
// ./minishell
