/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:34:17 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/01 20:42:22 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_heredoc(t_mini *var, char *delim, int fd, bool delim_quoted)
{
	char	*line_in;

	line_in = readline("> ");
	while (ft_strncmp(line_in, delim, ft_strlen(delim) + 1))
	{
		if (!line_in)
			error_exit(var, "heredoc delimited by end-of-file\n");
		if (!delim_quoted)
			line_in = expand_heredoc(var, line_in);
		write(fd, line_in, ft_strlen(line_in));
		write(fd, "\n", 1);
		free(line_in);
		line_in = readline("> ");
	}
	free(line_in);
}

void	heredoc_to_file(t_mini *var, char *filename, char **delim)
{
	int		fd;
	bool	delim_quoted;

	signals_heredoc();
	fd = open_tmp_file(var, filename);
	delim_quoted = rm_quotes_delim(var, delim);
	read_heredoc(var, *delim, fd, delim_quoted);
	close(fd);
	free(filename);
	free_var_exit(var, 0);
}

bool	heredoc(t_mini *var, t_token *delim)
{
	char	*file;
	pid_t	pid;
	int		status;

	file = get_tmp_file_name(var);
	if (!file)
		return (0);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), free(file), 0);
	else if (pid == 0)
		heredoc_to_file(var, file, &delim->content);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(file);
		free(file);
		var->exit_code = 130;
		return (0);
	}
	free(delim->content);
	delim->content = file;
	return (1);
}
