/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_prepare.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:46:40 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/05 14:46:40 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	already_has_path(t_mini *var, char **path, char *cmd)
{
	struct stat	path_stat;

	*path = ft_strdup(cmd);
	if (!*path)
		error_exit(var, "minishell: malloc failed\n");
	if (is_builtin(cmd))
		return (1);
	if (ft_strchr(cmd, '/'))
	{
		if (access(*path, F_OK) == -1)
			access_error(var, path, ": No such file or directory\n", 127);
		if (stat(*path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
			access_error(var, path, ": Is a directory\n", 126);
		if (access(*path, X_OK) == -1)
			access_error(var, path, ": Permission denied\n", 126);
		return (1);
	}
	return (0);
}

void	find_path_to_cmd(t_mini *var, char **path, char *cmd, int j)
{
	char	*tmp;

	if (!cmd || !*cmd)
		error_exit_code(var, "minishell: invalid command\n", 127);
	if (already_has_path(var, path, cmd))
		return ;
	while (var->paths && var->paths[j])
	{
		free(*path);
		tmp = ft_strjoin(var->paths[j++], "/");
		if (!tmp)
			error_exit(var, "minishell: malloc failed\n");
		*path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!*path)
			error_exit(var, "minishell: malloc failed\n");
		if (access(*path, X_OK) == 0)
			return ;
	}
	access_error(var, path, ": command not found\n", 127);
}

bool	cpy_content_to_argv(char **dst_argv, t_token *ptr,
		size_t argv_len)
{
	size_t	i;

	if (ptr->type != CMD)
	{
		write(2, "missing a command\n", 18);
		return (0);
	}
	dst_argv[0] = ptr->content;
	ptr = ptr->next;
	i = 1;
	while (i < argv_len)
	{
		if (ptr->type == WORD || ptr->type == FLAG)
		{
			dst_argv[i] = ptr->content;
			i++;
		}
		ptr = ptr->next;
	}
	dst_argv[i] = NULL;
	return (1);
}

t_token	*find_start_of_nth_cmd(t_mini *var, int cmd_n)
{
	t_token	*ptr;

	ptr = var->tokens;
	while (ptr && cmd_n > 0)
	{
		if (ptr->type == PIPE)
			cmd_n--;
		ptr = ptr->next;
	}
	if (!cmd_n)
		return (ptr);
	error_exit(var, "missing a command\n");
	return (NULL);
}

void	prepare_argv_and_redir(t_mini *var, int cmd_n)
{
	t_token	*ptr;
	t_token	*cmd;
	int		argv_len;

	redirect_in_out_to_pipes(var, cmd_n);
	ptr = find_start_of_nth_cmd(var, cmd_n);
	cmd = NULL;
	argv_len = redir_files_and_count_argv_len(ptr, &cmd, 1);
	if (argv_len == -1)
		free_var_exit(var, 1);
	var->argv_for_cmd = malloc((argv_len + 1) * sizeof(char *));
	if (!var->argv_for_cmd)
		error_exit(var, "malloc failed: prepare_argv_and_redir\n");
	if (!cpy_content_to_argv(var->argv_for_cmd, cmd, argv_len))
		free_var_exit(var, 1);
	find_path_to_cmd(var, &var->cmd, cmd->content, 0);
	close_pipes(var);
}
