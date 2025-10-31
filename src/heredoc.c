/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:34:17 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/31 17:40:10 by ksevciko         ###   ########.fr       */
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
		error_exit(var, "minishell: malloc failed\n");
	cpy_expanded_delim(*delim, result);
	free(*delim);
	*delim = result;
	return (quoted);
}

char	*cpy_expanded_heredoc(t_mini *var, char *str, char *result)
{
	int	dquote;
	int	squote;
	int	i;
	int	j;

	dquote = 0;
	squote = 0;
	i = -1;
	j = 0;
	while (str[++i])
	{
		if (str[i] == '"' && !squote)
			dquote = 1 - dquote;
		else if (str[i] == 39 && !dquote)
			squote = 1 - squote;
		else if (str[i] == '$' && !squote
			&& (ft_isalnum(str[i + 1]) || str[i + 1] == '?'))
		{
			j += cpy_env_var_delim(var, str, &i, &result[j]); //check if this returned -1
		}
		else
			result[j++] = str[i];
	}
	result[j] = '\0';
	return (result);
}

char	*expand_str_delim(t_mini *var, char *str) //todo: word splitting here, including detecting whethter the token cam be deleted (and only if it can = there are no "", run empty token in expand_tokens)
{
	int		len;
	char	*result;

	len = len_expanded_heredoc(var, str);
	result = (char *)malloc((len + 1) * sizeof(char));
	if (!result || len == -1)
	{
		free(result);
		write(2, "minishell: malloc failed\n", 25);
		return (NULL);
	}
	result = cpy_expanded_heredoc(var, str, result);
	return (result);
}

void	read_heredoc(t_mini *var, char *delim, int fd, bool delim_quoted)
{
	char	*line_in;

	line_in = readline("> ");
	while (ft_strncmp(line_in, delim, ft_strlen(delim) + 1))
	{
		if (!line_in)
			error_exit(var, "heredoc delimited by end-of-file\n");
		if (!delim_quoted)
			line_in = expand_str_delim(var, line_in);
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

	signals_heredoc(); // Set heredoc signal handler
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
	{
		perror("fork");
		free(file);
		return (0);
	}
	else if (pid == 0)
		heredoc_to_file(var, file, &delim->content);
	waitpid(pid, &status, 0);
	signals_setup();
		// Restore interactive signal handlers after heredoc completes
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(file);
		free(file);
		var->exit_code = 130;
		g_signal = 130;  // Set global signal flag
		signals_setup(); // Restore interactive signal handlers
		return (0);
	}
	free(delim->content);
	delim->content = file;
	return (1);
}
