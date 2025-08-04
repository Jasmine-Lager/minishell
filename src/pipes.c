/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:48:15 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/04 21:04:24 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_pipes(t_mini *var)
{
	int	i;

	var->pipes = malloc(var->nbr_pipes * sizeof(int [2]));
	if (!var->pipes)
		other_error(var, "pipex: malloc failed\n");
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

void	in_out_for_1st_cmd(t_mini *var) //only call if using <
{
	int	fd;

	fd = open(var->infile, O_RDONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		perror("failed to open input");
		free_var_exit(var, 1);
	}
	if (dup2(fd, 0) == -1 || dup2(var->pipes[0][1], 1) == -1)
	{
		dup2_error(var);
	}
	close(fd);
}

void    in_out_for_last_cmd(t_mini *var) //only call for >
{
    int fd;
	int	cmd_n;

	cmd_n = var->nbr_pipes + 1;
    fd = open(var->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("failed to open output");
        free_var_exit(var, 1);
    }
    if (dup2(var->pipes[cmd_n - 1][0], 0) == -1 || dup2(fd, 1) == -1)
    {
        dup2_error(var);
    }
    close(fd);
}

void	redirect_in_out_fds(t_mini *var, int cmd_n) //needs to be modified for >> and <<
{
	if (cmd_n == 0)
		in_out_for_1st_cmd(var);
	else if (cmd_n == var->nbr_pipes)
	{
		in_out_for_last_cmd(var);
	}
	else
	{
		if (dup2(var->pipes[cmd_n - 1][0], 0) == -1
			|| dup2(var->pipes[cmd_n][1], 1) == -1)
			dup2_error(var);
	}
}
