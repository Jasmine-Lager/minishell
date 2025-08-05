/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:29:08 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/05 10:29:08 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_command(t_mini *var)
{
	parse(var); //(handle quotes, $, >, <, >>, <<, |)
	if (var->nbr_pipes > 0)
	{
		create_pipes(var);
		execute_cmds(var);
	}
	// else
	// {
	// 	exec_cmd(var);
	// }
}
