/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:48:15 by ksevciko          #+#    #+#             */
/*   Updated: 2025/09/14 19:19:13 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_pipes(t_mini *var)
{
	int	i;

	var->pipes = malloc(var->nbr_pipes * sizeof(int [2]));
	if (!var->pipes)
		error_exit(var, "Malloc failed: create_pipes\n");
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
