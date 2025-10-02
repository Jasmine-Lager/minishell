/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:34:17 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/02 14:58:00 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_char(char *str, char old, char new)
{
    int i = 0;

    while (str && str[i])
    {
        if (str[i] == old)
            str[i] = new;
        i++;
    }
	return (str);
}

char	*ft_strjoin_four_strings(const char *s1, const char *s2,
			const char *s3, const char *s4)
{
	char	*result;
	char	*tmp;

	result = ft_strjoin(s1, s2);
	if (!result)
		return (NULL);
	tmp = ft_strjoin(result, s3);
	free(result);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, s4);
	free(tmp);
	if (!result)
		return (NULL);
	return (result);
}

char	*get_tmp_file_name(t_mini *var)
{
	char	*id_s;
	char	*file;
	char	*tty;

	tty = ttyname(0);
	if (!tty)
	{
		perror("error of function ttyname");
		return (NULL);
	}
	tty = replace_char(tty, '/', '_');
	id_s = ft_itoa(var->nbr_heredoc);
	if (!id_s)
		return (NULL);
	file = ft_strjoin_four_strings("/tmp/.minishell_heredoc_", id_s, "_", tty);
	free(id_s);
	if (!file)
		return (NULL);
	if (!*file)
	{
		free(file);
		return (NULL);
	}
	return (file);
}

void	heredoc(t_mini *var, t_token *delim)
{
	char	*file;
	pid_t	pid;

	file = get_tmp_file_name(var);
	if (!file)
		return ;
	var->nbr_heredoc++;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;//make it return all the way to main
	}
	else if (pid == 0 && !read_heredoc())
		free_var_exit(var, 1);
	free(delim->content);
	delim->content = file;
	delim->type = INFILE;
}
