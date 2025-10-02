/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 19:11:16 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/02 19:11:16 by ksevciko         ###   ########.fr       */
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
		free_var_exit(var, errno); //is in child process, can exit
	}
	return (fd);
}

int	len_expanded_heredoc(t_mini *var, char *str)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '?'))
			len += count_env_var_len(var, str, &i);
		else
			len++;
		i++;
	}
	return (len);
}

char	*cpy_expanded_heredoc(t_mini *var, char *str, char *result)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '?'))
			j += cpy_env_var(var, str, &i, &result[j]);
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	*expand_heredoc(t_mini *var, char *str)
{
	int		len;
	char	*result;

	len = len_expanded_heredoc(var, str);
	result = (char *)malloc((len + 1) * sizeof(char));
	if (!result)
		error_exit(var, BOLD RED "minishell: malloc failed\n" RESET);
	cpy_expanded_heredoc(var, str, result);
	return (result);
}
