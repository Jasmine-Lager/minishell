/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 15:49:55 by jasminelage       #+#    #+#             */
/*   Updated: 2025/11/11 12:33:01 by jlager           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// signo is the integer ID of the signal that triggered the handler
// Handler for interactive prompt (main shell)
void	handle_ctrl_c(int signal_number)
{
	(void)signal_number;
	g_signal = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// Handler during command execution (parent waits)
void	handle_ctrl_c_execution(int signal_number)
{
	(void)signal_number;
	g_signal = 130;
}

// Handler for heredoc input
void	handle_ctrl_c_heredoc(int signal_number)
{
	(void)signal_number;
	g_signal = 130;
	write(1, "\n", 1);
	exit(130);
}
