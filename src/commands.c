/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:29:08 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/02 12:52:53 by ksevciko         ###   ########.fr       */
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

t_token	*find_start_of_nth_cmd(t_mini *var, int cmd_n) //for prepare_argv_and_redir in execute.c, but will be needed for buid ins too
{
	t_token	*ptr;

	ptr = var->tokens;
	while (ptr && cmd_n > 0)
	{
		if (ptr->type == PIPE)
			cmd_n--;
		ptr = ptr->next;
	}
	if (!cmd_n)
		return (ptr);
	error_exit(var, "missing a command\n");
	return (NULL);
}

void	handle_command(t_mini *var)
{
	parse(var);
	if (!var->tokens)
		return ;
	// print_tokens(var->tokens);
	execute_cmds(var);
}

// echo with option -n
// cd with only a relative or absolute path
// pwd with no options
// export with no options
// unset with no options
// env with no options or arguments
// exit with no options
