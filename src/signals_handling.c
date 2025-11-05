/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 15:49:55 by jasminelage       #+#    #+#             */
/*   Updated: 2025/11/05 14:22:36 by ksevciko         ###   ########.fr       */
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
	write(1, "\n", 1);
}

// Handler for heredoc input
void	handle_ctrl_c_heredoc(int signal_number)
{
	(void)signal_number;
	g_signal = 130;
	write(1, "\n", 1);
	exit(130);
}
