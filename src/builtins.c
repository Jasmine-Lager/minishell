/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:08:44 by jasminelage       #+#    #+#             */
/*   Updated: 2025/11/08 00:27:15 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// echo with option -n
int	builtin_echo(char **argv)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (argv[i] && ft_strncmp(argv[i], "-n", 3) == 0)
	{
		n_flag = 1;
		i++;
	}
	while (argv[i])
	{
		write(1, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	return (0);
}

// cd with only a relative or absolute path
int	builtin_cd(t_mini *var, char **argv)
{
	char	*path;
	char	cwd[PATH_MAX];

	if (!argv[1])
	{
		path = find_env_var(var->envp, "HOME");
		if (!path)
			return (write(2, "minishell: cd: HOME not set\n", 28), 1);
	}
	else if (argv[2])
		return (write(2, "minishell: cd: too many arguments\n", 34), 1);
	else
		path = argv[1];
	if (chdir(path) == -1)
	{
		write(2, "minishell: cd: ", 15);
		perror(path);
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)))
		update_env_var(var, "PWD", cwd);
	return (0);
}

// pwd with no options
int	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
	{
		write(1, cwd, ft_strlen(cwd));
		write(1, "\n", 1);
		return (0);
	}
	perror("minishell: pwd");
	return (1);
}

// env with no options or arguments
int	builtin_env(t_mini *var, char **argv)
{
	int	i;

	if (argv[1])
	{
		write(2, "minishell: env: too many arguments\n", 35);
		return (1);
	}
	i = 0;
	while (var->envp[i])
	{
		write(1, var->envp[i], ft_strlen(var->envp[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}

// exit with no options
int	builtin_exit(t_mini *var, char **argv)
{
	int	exit_code;

	if (isatty(STDOUT_FILENO))
		write(2, "exit\n", 5);
	if (!argv[1])
		free_var_exit(var, var->exit_code);
	if (argv[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	exit_code = ft_atoi(argv[1]);
	if (!is_valid_number(argv[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": numeric argument required\n", 28);
		free_var_exit(var, 2);
	}
	free_var_exit(var, exit_code % 256);
	return (0);
}
