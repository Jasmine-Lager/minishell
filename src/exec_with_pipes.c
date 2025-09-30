/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_with_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:59:49 by ksevciko          #+#    #+#             */
/*   Updated: 2025/09/30 22:27:51 by ksevciko         ###   ########.fr       */
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

	if (!dst_argv)
		error_exit(var, "malloc failed: find_nth_cmd_and_argv\n");
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

t_token	*find_start_of_nth_cmd(t_mini *var, int cmd_n)
{
	t_token *ptr;

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

void redirect_in_out_to_pipes(t_mini *var, int cmd_n)
{
	int	in;
	int	out;

	if (cmd_n == 0)
		in = 0;
	else
		in = var->pipes[cmd_n - 1][0];
	if (cmd_n == var->nbr_pipes)
		out = 1;
	else
		out = var->pipes[cmd_n][1];
	if (dup2(in, 0) == -1
		|| dup2(out, 1) == -1)
	{
		dup2_error(var);
	}
}

void open_redir_infile(t_mini *var, char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		perror("failed to open input");
		free_var_exit(var, 1);
	}
	if (dup2(fd, 0) == -1) //is this okay with dup2? sdince i already redirected it? should i maybe somehow close the last file i did this for first?
		dup2_error(var);
	close(fd);
}

void open_redir_outfile(t_mini *var, char *outfile, bool append)
{
	int	fd;

	if (!append)
		fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("failed to open output");
		free_var_exit(var, 1);
	}
	if (dup2(fd, 0) == -1)
		dup2_error(var);
	close(fd);
}

int	redir_files_and_count_argv_len(t_mini *var, t_token *ptr,
		t_token **cmd, int argv_len)
{
	bool	append;

	append = 0;
	while (ptr)
	{
		if (ptr->type == WORD || ptr->type == FLAG)
			argv_len++;
		else if (ptr->type == CMD)
			*cmd = ptr;
		else if (ptr->type == PIPE)
			break ;
		else if (ptr->type == INFILE)
			open_redir_infile(var, ptr->content); //close those files here too!
		else if (ptr->type == REDIR_OUT)
			append = 0;
		else if (ptr->type == REDIR_APPEND)
			append = 1;
		else if (ptr->type == OUTFILE)
			open_redir_outfile(var, ptr->content, append); //also use close on the files!
		ptr = ptr->next;
	}
	if (!*cmd)
		error_exit(var, "missing a command\n");
	return (argv_len);
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
