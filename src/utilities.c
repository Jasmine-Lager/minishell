/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:24 by jlager            #+#    #+#             */
/*   Updated: 2025/09/25 18:55:52 by jlager           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// the string says what message the error should display
void error_exit(t_mini *var, char *str)
{
	write(2, str, ft_strlen(str));
	free_var_exit(var, 1);
}

void command_not_found(t_mini *var, char **path)
{
	free(*path);
	*path = NULL;
	perror("command not found");
	free_var_exit(var, 127);
}

void dup2_error(t_mini *var)
{
	perror("dup2");
	free_var_exit(var, 1);
}
