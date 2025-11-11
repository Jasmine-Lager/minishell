/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 09:09:21 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/11 13:30:26 by ksevciko         ###   ########.fr       */
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
	if (dup2(in, 0) == -1 || dup2(out, 1) == -1)
		dup2_error(var);
}

bool	open_redir_infile(char *infile, bool heredoc)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd == -1)
	{
		perror(infile);
		return (0);
	}
	if (dup2(fd, 0) == -1)
	{
		perror("dup2");
		return (0);
	}
	close(fd);
	if (heredoc)
		unlink(infile);
	return (1);
}

bool	open_redir_outfile(char *outfile, bool append)
{
	int	fd;

	if (!append)
		fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(outfile);
		return (0);
	}
	if (dup2(fd, 1) == -1)
	{
		perror("dup2");
		return (0);
	}
	close(fd);
	return (1);
}

void	process_cmd(t_token *ptr, t_token **cmd, int *argv_len)
{
	if (!*cmd)
		*cmd = ptr;
	else
	{
		ptr->type = WORD;
		(*argv_len)++;
	}
}

int	redir_files_and_count_argv_len(t_token *ptr, t_token **cmd, int argv_len)
{
	bool	append;

	append = 0;
	while (ptr && ptr->type != PIPE)
	{
		if (ptr->type == WORD || ptr->type == FLAG)
			argv_len++;
		else if (ptr->type == CMD)
			process_cmd(ptr, cmd, &argv_len);
		else if ((ptr->type == INFILE || ptr->type == DELIMITER)
			&& !open_redir_infile(ptr->content, ptr->type == DELIMITER))
			return (-1);
		else if (ptr->type == REDIR_OUT)
			append = 0;
		else if (ptr->type == REDIR_APPEND)
			append = 1;
		else if ((ptr->type == OUTFILE)
			&& !open_redir_outfile(ptr->content, append))
			return (-1);
		ptr = ptr->next;
	}
	return (argv_len);
}
