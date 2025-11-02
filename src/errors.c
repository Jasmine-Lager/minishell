/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:24 by jlager            #+#    #+#             */
/*   Updated: 2025/11/02 01:52:21 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// the string says what message the error should display
void	error_exit(t_mini *var, char *str)
{
	write(2, str, ft_strlen(str));
	free_var_exit(var, 1);
}

void	error_exit_code(t_mini *var, char *str, int code)
{
	write(2, str, ft_strlen(str));
	free_var_exit(var, code);
}

void	command_not_found(t_mini *var, char **path)
{
	write(2, var->argv_for_cmd[0], ft_strlen(var->argv_for_cmd[0]));
	free(*path);
	*path = NULL;
	write(2, ": command not found\n", 20);
	free_var_exit(var, 127);
}

void	dup2_error(t_mini *var)
{
	perror("dup2");
	free_var_exit(var, 1);
}
