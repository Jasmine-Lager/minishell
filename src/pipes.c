/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:48:15 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/31 17:02:55 by jlager           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	create_pipes(t_mini *var)
{
	int	i;

	if (var->nbr_pipes > 0)
	{
		var->pipes = malloc(var->nbr_pipes * sizeof(int[2]));
		if (!var->pipes)
		{
			write(2, "Malloc failed: create_pipes\n", 28);
			return (0);
		}
	}
	i = 0;
	while (i < var->nbr_pipes)
	{
		if (pipe(var->pipes[i]) == -1)
		{
			perror("error creating pipe");
			return (0);
		}
		i++;
	}
	return (1);
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
