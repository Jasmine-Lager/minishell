/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:29:08 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/01 18:55:25 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	print_tokens(t_token *tokens)
// {
// 	while (tokens)
// 	{
// 		ft_printf("%s (%i)\n", tokens->content, tokens->type);
// 		tokens = tokens->next;
// 	}
// }

//a function that will check if the current command is built-in
//and if it is, it will call functions to execute it, will be 
//called in execute.c in function prepare_argv_and_redir.
//built ins must call exit after execution or on error

void	handle_command(t_mini *var)
{
	add_history(var->line);
	if (!parse(var) || !var->tokens)
		return ;
	// print_tokens(var->tokens);
	if (!execute_cmds(var))
		return ;
	if (g_signal == 130) // Ctrl+C was pressed - check here after running commands so minishell ends with correnct exit code when ctrl d is pressed?? 
	{
		var->exit_code = 130;
		g_signal = 0;
	}
}

// echo with option -n
// cd with only a relative or absolute path
// pwd with no options
// export with no options
// unset with no options
// env with no options or arguments
// exit with no options
