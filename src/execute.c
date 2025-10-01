/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:59:49 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/01 19:17:33 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_path_to_cmd(t_mini *var, char **path, char *cmd)
{
	char	*tmp;
	int		j;

	if (!cmd || !*cmd)
		error_exit(var, BOLD RED "minishell: invalid command\n" RESET);
	*path = ft_strdup(cmd);
	if (!*path)
		error_exit(var, BOLD RED "minishell: malloc failed\n" RESET);
	j = 0;
	while (access(*path, X_OK) == -1 && var->paths && var->paths[j])
	{
		free(*path);
		tmp = ft_strjoin(var->paths[j++], "/");
		*path = ft_strjoin(tmp, cmd);
		if (!tmp || !*path)
		{
			free(tmp);
			free(*path);
			*path = NULL;
			error_exit(var, BOLD RED "minishell: malloc failed\n" RESET);
		}
		free(tmp);
	}
	if (access(*path, X_OK) == -1)
		command_not_found(var, path);
}

void	cpy_content_to_argv(t_mini *var, char **dst_argv, t_token *ptr,
			size_t argv_len)
{
	size_t	i;

	if (ptr->type != CMD) //should never happen
		error_exit(var, "missing a command\n");
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
}

void	prepare_argv_and_redir(t_mini *var, int cmd_n)
{
	t_token	*ptr;
	t_token	*cmd;
	int		argv_len;

	redirect_in_out_to_pipes(var, cmd_n);
	ptr = find_start_of_nth_cmd(var, cmd_n);
	cmd = NULL;
	argv_len = redir_files_and_count_argv_len(var, ptr, &cmd, 1);
	var->argv_for_cmd = malloc((argv_len + 1) * sizeof(char *));
	if (!var->argv_for_cmd)
		error_exit(var, "malloc failed: prepare_argv_and_redir\n");
	cpy_content_to_argv(var, var->argv_for_cmd, cmd, argv_len);
	find_path_to_cmd(var, &var->cmd, cmd->content);
}

void	wait_for_children(t_mini *var, pid_t last_child_pid)
{
	int	i;
	int	status;

	i = 0;
	while (i <= var->nbr_pipes)
	{
		if (wait(&status) == last_child_pid)
		{
			if (WIFEXITED(status))
				var->exit_code = WEXITSTATUS(status);
			else
				var->exit_code = 1;
		}
		i++;
	}
}

void	execute_cmds(t_mini *var)
{
	int		n;
	pid_t	pid;

	create_pipes(var);
	n = 0;
	while (n <= var->nbr_pipes)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free_var_exit(var, 1);
		}
		else if (pid == 0)
		{
			prepare_argv_and_redir(var, n);
			close_pipes(var);
			execve(var->cmd, var->argv_for_cmd, var->envp);
			perror("execve");
			free_var_exit(var, 1);
		}
		n++;
	}
	close_pipes(var);
	wait_for_children(var, pid);
}
