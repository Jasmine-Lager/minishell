/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 19:11:16 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/31 17:02:29 by jlager           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_tmp_file_name(t_mini *var)
{
	char	*id_str;
	char	*file;
	char	*tty;

	tty = ttyname(0);
	if (!tty)
	{
		perror("error of function ttyname");
		return (NULL);
	}
	tty = replace_char(tty, '/', '_');
	id_str = ft_itoa(var->nbr_heredoc);
	if (!id_str)
		return (NULL);
	file = ft_strjoin_four("/tmp/.minishell_heredoc_", id_str, "_", tty);
	free(id_str);
	if (!file)
		return (NULL);
	if (!*file)
	{
		free(file);
		return (NULL);
	}
	var->nbr_heredoc++;
	return (file);
}

int	open_tmp_file(t_mini *var, char *filename)
{
	int	fd;

	fd = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0600);
	if (fd == -1)
	{
		if (errno == EEXIST)
			perror(filename);
		else
			perror("open failed");
		free_var_exit(var, errno); // is in child process, can exit
	}
	return (fd);
}
