/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:29:08 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/13 13:42:13 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_command(t_mini *var)
{
	parse(var); //(handle quotes, $, >, <, >>, <<, |) //if here_doc, nbr_pipes++
	// if (var->nbr_pipes > 0)
	// {
	// 	create_pipes(var);
	// 	execute_cmds(var);
	// }
	// else
	// {
	// 	exec_cmd(var);
	// }
}

// echo with option -n
// cd with only a relative or absolute path
// pwd with no options
// export with no options
// unset with no options
// env with no options or arguments
// exit with no options
