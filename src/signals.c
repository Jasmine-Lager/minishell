/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:33 by jlager            #+#    #+#             */
/*   Updated: 2025/11/11 16:58:28 by jlager           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Setup for interactive prompt
// Important: restart interrupted system calls
void	signals_setup(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
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
// No SA_RESTART for heredoc - we want it to interrupt
void	signals_heredoc(void)
{
	struct sigaction	sa;
	struct termios		term;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handle_ctrl_c_heredoc;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Setup for child processes
void	signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
