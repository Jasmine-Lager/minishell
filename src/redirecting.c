/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirecting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 09:09:21 by ksevciko          #+#    #+#             */
/*   Updated: 2025/08/05 09:09:21 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	in_out_for_1st_cmd(t_mini *var)
{
	int	fd;

	if (var->infile)
		fd = open(var->infile, O_RDONLY | O_CREAT, 0644);
	else
		fd = 0;
	if (fd == -1)
	{
		perror("failed to open input");
		free_var_exit(var, 1);
	}
	if (dup2(fd, 0) == -1 || dup2(var->pipes[0][1], 1) == -1)
	{
		dup2_error(var);
	}
	if (fd != 0)
		close(fd);
}

void	in_out_for_last_cmd(t_mini *var)
{
	int	fd;

	if (var->outfile && !var->append_mode)
		fd = open(var->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (var->outfile)
		fd = open(var->outfile, O_WRONLY | O_CREAT | O_APPEND , 0644);
	else
		fd = 1;
	if (fd == -1)
	{
		perror("failed to open output");
		free_var_exit(var, 1);
	}
	if (dup2(var->pipes[var->nbr_pipes - 1][0], 0) == -1 || dup2(fd, 1) == -1)
	{
		dup2_error(var);
	}
	if (fd != 1)
		close(fd);
}

void	here_doc(t_mini *var)
{
	char	*line_in;

	line_in = readline(">");
	while (ft_strncmp(line_in, var->delimiter, ft_strlen(var->delimiter)))
	{
		if (!line_in)
			error_exit(var, "minishell: here-document delimited by EOF\n");
		ft_printf("%s\n", line_in);
		free(line_in);
		line_in = readline(">");
	}
	free(line_in);
	free_var_exit(var, 0);
}

void	redirect_for_pipes(t_mini *var, int cmd_n)
{
	if (cmd_n == 0 && !var->here_doc)
		in_out_for_1st_cmd(var);
	else if (cmd_n == 0 && var->here_doc)
	{
		if (dup2(var->pipes[0][1], 1) == -1)
		{
			dup2_error(var);
		}
		here_doc(var);
	}
	else if (cmd_n == var->nbr_pipes)
	{
		in_out_for_last_cmd(var);
	}
	else
	{
		if (dup2(var->pipes[cmd_n - 1][0], 0) == -1
			|| dup2(var->pipes[cmd_n][1], 1) == -1)
		{
			dup2_error(var);
		}
		if (cmd_n == 1 && var->here_doc)
			wait();
	}
}
