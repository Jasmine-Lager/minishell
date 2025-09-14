/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:33 by jlager            #+#    #+#             */
/*   Updated: 2025/09/14 22:03:14 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ctrl+C: interrupts the current input and redisplays the prompts (SIGINT)
//		but not the shell itself.
// Ctrl+\: Normally quits, here it should not do anything (SIGQUIT)
// Ctrl+D and EOF: Exit shell, Check for NULL/0 from input (in main)

// write(1, "\n", 1) ensures the cursor moves to a new line.
// 	rl_replace_line("", 0) clears the current buffer
// 	rl_on_new_line() tells readline a new line started
// 	rl_redisplay() reprints the prompt and the (now empty) buffer

// signo is the integer ID of the signal that triggered the handler
void handle_ctrl_c(int signal_number)
{
	if (signal_number == SIGINT)
	{
		g_signal = SIGINT;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void signals_setup(void)
{
	signal(SIGINT, handle_ctrl_c); // Ctrl-C
	signal(SIGQUIT, SIG_IGN);	   // Ctrl-\ (ignoring)
}
