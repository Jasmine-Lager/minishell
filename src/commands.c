/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:29:08 by ksevciko          #+#    #+#             */
/*   Updated: 2025/09/18 14:09:01 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void print_tokens(t_token *tokens)
// {
// 	while (tokens)
// 	{
// 		ft_printf("%s (%i)\n", tokens->content, tokens->type);
// 		tokens = tokens->next;
// 	}
// }

void	handle_command(t_mini *var)
{
	parse(var);
	// print_tokens(var->tokens);
	if (var->nbr_pipes > 0)
	{
		create_pipes(var);
		execute_cmds(var);
	}
	else
	{
		execute_cmd(var);
	}
}

// echo with option -n
// cd with only a relative or absolute path
// pwd with no options
// export with no options
// unset with no options
// env with no options or arguments
// exit with no options
