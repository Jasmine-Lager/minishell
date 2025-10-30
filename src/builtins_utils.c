/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:08:50 by jasminelage       #+#    #+#             */
/*   Updated: 2025/10/30 15:23:37 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

// Check if string is a valid number for exit
int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

// Check if command is a built-in
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

// Execute built-in command
int	execute_builtin(t_mini *var, char **argv)
{
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		return (builtin_echo(argv));
	if (ft_strncmp(argv[0], "cd", 3) == 0)
		return (builtin_cd(var, argv));
	if (ft_strncmp(argv[0], "pwd", 4) == 0)
		return (builtin_pwd());
	if (ft_strncmp(argv[0], "export", 7) == 0)
		return (builtin_export(var, argv));
	if (ft_strncmp(argv[0], "unset", 6) == 0)
		return (builtin_unset(var, argv));
	if (ft_strncmp(argv[0], "env", 4) == 0)
		return (builtin_env(var, argv));
	if (ft_strncmp(argv[0], "exit", 5) == 0)
		return (builtin_exit(var, argv));
	return (0);
}
