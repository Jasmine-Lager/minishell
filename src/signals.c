/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:33 by jlager            #+#    #+#             */
/*   Updated: 2025/08/06 20:43:19 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ctrl+C: interrupts the current input and redisplays the prompts (SIGINT)
// but not the shell itself.
// Ctrl+\: Normally quits, here it should not do anything (SIGQUIT)
// Ctrl+D and EOF: Exit shell, Check for NULL/0 from input

// void	handle_ctrl_c(int ctrl_c)
// {
// }

// void	setup_signals(void)
// {
// 	signal(SIGINT, handle_sigint); // Ctrl-C
// 	signal(SIGQUIT, SIG_IGN);      // Ctrl-\ (ignoreing by )
// }
