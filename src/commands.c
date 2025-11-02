/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:29:08 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/02 18:27:06 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_command(t_mini *var)
{
	add_history(var->line);
	if (!parse(var) || !var->tokens)
	{
		if (g_signal == 130)
		{
			var->exit_code = 130;
			g_signal = 0;
		}
		return ;
	}
	if (!execute_cmds(var))
	{
		if (g_signal == 130)
		{
			var->exit_code = 130;
			g_signal = 0;
		}
		return ;
	}
	if (g_signal == 130)
	{
		var->exit_code = 130;
		g_signal = 0;
	}
}

// static void	print_tokens(t_token *tokens)
// {
// 	while (tokens)
// 	{
// 		ft_printf("%s (%i)\n", tokens->content, tokens->type);
// 		tokens = tokens->next;
// 	}
// }