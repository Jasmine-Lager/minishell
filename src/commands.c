/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:29:08 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/31 16:57:45 by jlager           ###   ########.fr       */
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
	if (!parse(var) || !var->tokens)
		return ;
	// print_tokens(var->tokens);
	if (!execute_cmds(var))
		return ;
}

// echo with option -n
// cd with only a relative or absolute path
// pwd with no options
// export with no options
// unset with no options
// env with no options or arguments
// exit with no options
