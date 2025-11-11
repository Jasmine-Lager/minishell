/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 19:11:16 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/11 14:01:35 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_tty_name_no_slashes(void)
{
	char	*tty;
	char	*tty_name;

	tty_name = ttyname(0);
	if (!tty_name)
	{
		perror("error of function ttyname");
		return (NULL);
	}
	tty = ft_strdup(tty_name);
	if (!tty)
	{
		write(2, "malloc failed\n", 25);
		return (NULL);
	}
	tty = replace_char(tty, '/', '_');
	return (tty);
}

char	*get_tmp_file_name(t_mini *var)
{
	char	*id_str;
	char	*file;
	char	*tty;

	tty = get_tty_name_no_slashes();
	if (!tty)
		return (NULL);
	id_str = ft_itoa(var->nbr_heredoc);
	if (!id_str)
		return (free(tty), NULL);
	file = ft_strjoin_four("/tmp/.minishell_heredoc_", id_str, "_", tty);
	free(id_str);
	free(tty);
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

int	open_tmp_file(t_mini *var, char *filename, t_expand *exp)
{
	int	fd;

	fd = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0600);
	if (fd == -1)
	{
		if (errno == EEXIST)
			perror(filename);
		else
			perror("open failed");
		free(filename);
		free(exp);
		free_var_exit(var, errno);
	}
	return (fd);
}
