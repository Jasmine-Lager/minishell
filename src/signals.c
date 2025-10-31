/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:33 by jlager            #+#    #+#             */
/*   Updated: 2025/10/31 17:02:32 by jlager           ###   ########.fr       */
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
	// Don't redisplay prompt during execution
}

// Handler for heredoc input
void	handle_ctrl_c_heredoc(int signal_number)
{
	(void)signal_number;
	g_signal = 130;
	write(1, "\n", 1);
	// Exit from heredoc child process
	exit(130);
}

// Setup for interactive prompt
void	signals_setup(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART; // Important: restart interrupted system calls
	sa.sa_handler = handle_ctrl_c;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

// Setup during command execution
void	signals_execution(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handle_ctrl_c_execution;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

// Setup for heredoc
void	signals_heredoc(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0; // No SA_RESTART for heredoc - we want it to interrupt
	sa.sa_handler = handle_ctrl_c_heredoc;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

// Setup for child processes
void	signals_child(void)
{
	signal(SIGINT, SIG_DFL); // Default handling
	signal(SIGQUIT, SIG_DFL);
}
