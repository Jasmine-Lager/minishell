/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:33 by jlager            #+#    #+#             */
/*   Updated: 2025/08/12 12:54:10 by jlager           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ctrl+C: interrupts the current input and redisplays the prompts (SIGINT)
// but not the shell itself.
// Ctrl+\: Normally quits, here it should not do anything (SIGQUIT)
// Ctrl+D and EOF: Exit shell, Check for NULL/0 from input

// write(1, "\n", 1) ensures the cursor moves to a new line.
// 	rl_replace_line("", 0) clears the current buffer
// 	rl_on_new_line() tells readline a new line started
// 	rl_redisplay() reprints the prompt and the (now empty) buffer

// signo is the integer ID of the signal that triggered the handler
void	handle_ctrl_c(int signal_number)
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

void	setup_signals(void)
{
	signal(SIGINT, handle_ctrl_c); // Ctrl-C
	signal(SIGQUIT, SIG_IGN);      // Ctrl-\ (ignoreing by )
}

// Common pitfalls to avoid
// Don’t print the prompt manually in the handler; let readline redisplay it.

// Don’t call printf/ft_printf in the handler.

// Ensure SIGQUIT is ignored in the interactive parent so Ctrl-\ doesn’t dump
// core in the shell.

// Restore default signals in child processes before exec so Ctrl-C affects
// the running command as expected (parent stays interactive).
