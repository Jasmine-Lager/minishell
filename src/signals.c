/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:33 by jlager            #+#    #+#             */
/*   Updated: 2025/11/04 15:49:35 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
