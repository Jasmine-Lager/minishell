/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:34:17 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/02 20:29:51 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_no_quotes(char *delim, bool *quoted)
{
	int	len;
	int	dquote;
	int	squote;
	int	i;

	len = 0;
	dquote = 0;
	squote = 0;
	i = -1;
	while (delim[++i])
	{
		if (delim[i] == '"' && !squote)
		{
			dquote = 1 - dquote;
			*quoted = 1;
		}
		else if (delim[i] == 39 && !dquote)
		{
			squote = 1 - squote;
			*quoted = 1;
		}
		else
			len++;
	}
	return (len);
}

bool	rm_quotes_delim(t_mini *var, char **delim)
{
	int		len;
	char	*result;
	bool	quoted;

	quoted = 0;
	len = len_no_quotes(*delim, &quoted);
	result = (char *)malloc((len + 1) * sizeof(char));
	if (!result)
		error_exit(var, BOLD RED "minishell: malloc failed\n" RESET);
	cpy_expanded(var, *delim, result, 1);
	free(*delim);
	*delim = result;
	return (quoted);
}

void	read_heredoc(t_mini *var, char *delim, int fd, bool delim_quoted)
{
	char	*line_in;

	line_in = readline(BOLD GRAY ">" RESET);
	while (ft_strncmp(line_in, delim, ft_strlen(delim) + 1))
	{
		if (!line_in)
			error_exit(var, "heredoc delimited by end-of-file\n");
		if (!delim_quoted)
			line_in = expand_str(var, line_in);
		write(fd, line_in, ft_strlen(line_in));
		write(fd, "\n", 1);
		free(line_in);
		line_in = readline(BOLD GRAY ">" RESET);
	}
	free(line_in);
}

void	heredoc_to_file(t_mini *var, char *filename, char **delim)
{
	int		fd;
	bool	delim_quoted;

	fd = open_tmp_file(var, filename);
	delim_quoted = rm_quotes_delim(var, delim);
	read_heredoc(var, *delim, fd, delim_quoted);
	close(fd);
	free(filename);
	free_var_exit(var, 0);
}

void	heredoc(t_mini *var, t_token *delim)
{
	char	*file;
	pid_t	pid;
	int		status;

	file = get_tmp_file_name(var);
	if (!file)
		return ;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ; //make it return all the way to main
	}
	else if (pid == 0)
		heredoc_to_file(var, file, &delim->content);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status)) // kinda what a signal should do here (AI generated, please check it)
	{
		unlink(file);
		g_signal = 130;  // 128 + SIGINT
		return ;  // return all the way to main immediately, do not execute anything
	}
	free(delim->content);
	delim->content = file;
}
