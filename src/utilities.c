/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:24 by jlager            #+#    #+#             */
/*   Updated: 2025/09/14 16:31:37 by ksevciko         ###   ########.fr       */
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
	perror("Command not found");
	free_var_exit(var, 127);
}

void dup2_error(t_mini *var)
{
	perror("dup2");
	free_var_exit(var, 1);
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		ft_printf("%s (%i)\n", tokens->content, tokens->type);
		tokens = tokens->next;
	}
}
