/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:48:15 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/13 12:59:37 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_pipes(t_mini *var)
{
	int	i;

	var->pipes = malloc(var->nbr_pipes * sizeof(int [2]));
	if (!var->pipes)
		error_exit(var, "pipex: malloc failed\n");
	i = 0;
	while (i < var->nbr_pipes)
	{
		if (pipe(var->pipes[i]) == -1)
		{
			perror("error creating pipe");
			free_var_exit(var, 1);
		}
		i++;
	}
}

void	close_pipes(t_mini *var)
{
	int	j;

	j = 0;
	while (j < var->nbr_pipes)
	{
		close(var->pipes[j][0]);
		close(var->pipes[j][1]);
		j++;
	}
}
