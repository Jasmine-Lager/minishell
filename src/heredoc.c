/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:34:17 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/11 16:22:13 by jlager           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_heredoc(t_mini *var, char *delim, t_expand *exp, bool delim_quoted)
{
	char	*line_in;

	line_in = readline("> ");
	while (ft_strncmp(line_in, delim, ft_strlen(delim) + 1))
	{
		if (!line_in)
		{
			free(exp);
			free(line_in);
			error_exit(var, "heredoc delimited by end-of-file\n");
		}
		if (!delim_quoted)
			line_in = expand_heredoc(var, line_in, exp);
		write(exp->fd, line_in, ft_strlen(line_in));
		write(exp->fd, "\n", 1);
		free(line_in);
		line_in = readline("> ");
	}
	free(line_in);
}

void	heredoc_to_file(t_mini *var, char *filename, char **delim,
		t_expand *exp)
{
	bool	delim_quoted;

	signals_heredoc();
	exp->fd = open_tmp_file(var, filename, exp);
	free(filename);
	delim_quoted = rm_quotes_delim(var, delim, exp);
	read_heredoc(var, *delim, exp, delim_quoted);
	close(exp->fd);
	free(exp);
	free_var_exit(var, 0);
}

bool	heredoc(t_mini *var, t_token *delim, t_expand *exp)
{
	char	*file;
	pid_t	pid;
	int		status;

	file = get_tmp_file_name(var);
	if (!file)
		return (0);
	signals_execution();
	pid = fork();
	if (pid == -1)
		return (perror("fork"), free(file), 0);
	else if (pid == 0)
		heredoc_to_file(var, file, &delim->content, exp);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(file);
		free(file);
		var->exit_code = 130;
		return (signals_setup(), 0);
	}
	free(delim->content);
	delim->content = file;
	signals_setup();
	return (1);
}
