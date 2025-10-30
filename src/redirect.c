/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 09:09:21 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/30 15:30:14 by jasminelage      ###   ########.fr       */
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

void	open_redir_infile(t_mini *var, char *infile, bool heredoc)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd == -1)
	{
		perror(infile);
		free_var_exit(var, 1);
	}
	if (dup2(fd, 0) == -1)
		dup2_error(var);
	close(fd);
	if (heredoc)
		unlink(infile);
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
		else if (ptr->type == INFILE || ptr->type == DELIMITER)
			open_redir_infile(var, ptr->content, ptr->type == DELIMITER);
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
