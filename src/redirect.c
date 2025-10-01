/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 09:09:21 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/01 19:09:26 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_in_out_to_pipes(t_mini *var, int cmd_n)
{
	int	in;
	int	out;

	if (cmd_n == 0)
		in = 0;
	else
		in = var->pipes[cmd_n - 1][0];
	if (cmd_n == var->nbr_pipes)
		out = 1;
	else
		out = var->pipes[cmd_n][1];
	if (dup2(in, 0) == -1
		|| dup2(out, 1) == -1)
	{
		dup2_error(var);
	}
}

void	open_redir_infile(t_mini *var, char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd == -1)
	{
		perror("failed to open input");
		free_var_exit(var, 1);
	}
	if (dup2(fd, 0) == -1)
		dup2_error(var);
	close(fd);
}

void	open_redir_outfile(t_mini *var, char *outfile, bool append)
{
	int	fd;

	if (!append)
		fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("failed to open output");
		free_var_exit(var, 1);
	}
	if (dup2(fd, 1) == -1)
		dup2_error(var);
	close(fd);
}

void	process_cmd(t_token *ptr, t_token **cmd, int *argv_len)
{
	if (!*cmd)
	{
		*cmd = ptr;
	}
	else
	{
		ptr->type = WORD;
		(*argv_len)++;
	}
}

int	redir_files_and_count_argv_len(t_mini *var, t_token *ptr,
		t_token **cmd, int argv_len)
{
	bool	append;

	append = 0;
	while (ptr)
	{
		if (ptr->type == WORD || ptr->type == FLAG)
			argv_len++;
		else if (ptr->type == CMD)
			process_cmd(ptr, cmd, &argv_len);
		else if (ptr->type == PIPE)
			break ;
		else if (ptr->type == INFILE)
			open_redir_infile(var, ptr->content);
		else if (ptr->type == REDIR_OUT)
			append = 0;
		else if (ptr->type == REDIR_APPEND)
			append = 1;
		else if (ptr->type == OUTFILE)
			open_redir_outfile(var, ptr->content, append);
		ptr = ptr->next;
	}
	if (!*cmd)
		error_exit(var, "missing a command\n");
	return (argv_len);
}

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
