// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   redirecting.c                                      :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/08/05 09:09:21 by ksevciko          #+#    #+#             */
// /*   Updated: 2025/08/05 09:09:21 by ksevciko         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"

// void	in_out_for_1st_cmd(t_mini *var)
// {
// 	int	fd;

// 	if (var->current->infile)
// 		fd = open(var->current->infile, O_RDONLY | O_CREAT, 0644);
// 	else
// 		fd = 0;
// 	if (fd == -1)
// 	{
// 		perror("failed to open input");
// 		free_var_exit(var, 1);
// 	}
// 	if (dup2(fd, 0) == -1 || dup2(var->pipes[0][1], 1) == -1)
// 		dup2_error(var);
// 	if (fd != 0)
// 		close(fd);
// }

// void	in_out_for_last_cmd(t_mini *var)
// {
// 	int	fd;

// 	if (var->current->outfile && !var->current->append_mode)
// 		fd = open(var->current->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	else if (var->current->outfile)
// 		fd = open(var->current->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	else
// 		fd = 1;
// 	if (fd == -1)
// 	{
// 		perror("failed to open output");
// 		free_var_exit(var, 1);
// 	}
// 	if (dup2(var->pipes[var->nbr_pipes - 1][0], 0) == -1 || dup2(fd, 1) == -1)
// 		dup2_error(var);
// 	if (fd != 1)
// 		close(fd);
// }

// void	here_doc(t_mini *var)
// {
// 	char	*line_in;

// 	line_in = readline(BOLD GRAY ">" RESET);
// 	while (ft_strncmp(line_in, var->current->delimiter,
// 			ft_strlen(var->current->delimiter) + 1))
// 	{
// 		if (line_in)
// 		{
// 			// if (!var->delim_quoted)
// 			// 	expand(var, line_in);
// 			write(var->pipes[0][1], line_in, ft_strlen(line_in));
// 			write(var->pipes[0][1], "\n", 1);
// 			free(line_in);
// 		}
// 		line_in = readline(BOLD GRAY ">" RESET);
// 	}
// 	free(line_in);
// }

// void	redirect_for_pipes(t_mini *var, int cmd_n)
// {
// 	if (cmd_n == 1 && var->current->here_doc)
// 		here_doc(var);
// 	if (cmd_n == 0)
// 		in_out_for_1st_cmd(var);
// 	else if (cmd_n == var->nbr_pipes)
// 		in_out_for_last_cmd(var);
// 	else
// 	{
// 		if (dup2(var->pipes[cmd_n - 1][0], 0) == -1
// 			|| dup2(var->pipes[cmd_n][1], 1) == -1)
// 			dup2_error(var);
// 	}
// }
