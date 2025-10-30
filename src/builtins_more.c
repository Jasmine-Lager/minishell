/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_more.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:10:22 by jasminelage       #+#    #+#             */
/*   Updated: 2025/10/30 15:23:49 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper to check if identifier is valid (alphanumeric or underscore)
static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// Print export without arguments (sorted list of all env vars)
static void	print_export(t_mini *var)
{
	int		i;
	char	*equal_sign;

	i = 0;
	while (var->envp[i])
	{
		write(1, "declare -x ", 11);
		equal_sign = ft_strchr(var->envp[i], '=');
		if (equal_sign)
		{
			write(1, var->envp[i], equal_sign - var->envp[i] + 1);
			write(1, "\"", 1);
			write(1, equal_sign + 1, ft_strlen(equal_sign + 1));
			write(1, "\"", 1);
		}
		else
			write(1, var->envp[i], ft_strlen(var->envp[i]));
		write(1, "\n", 1);
		i++;
	}
}

// export with no options
int	builtin_export(t_mini *var, char **argv)
{
	int		i;
	char	*equal_sign;
	char	*key;
	char	*value;
	int		key_len;

	if (!argv[1])
	{
		print_export(var);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			write(2, "minishell: export: `", 20);
			write(2, argv[i], ft_strlen(argv[i]));
			write(2, "': not a valid identifier\n", 26);
			i++;
			continue;
		}
		equal_sign = ft_strchr(argv[i], '=');
		if (equal_sign)
		{
			key_len = equal_sign - argv[i];
			key = malloc(key_len + 1);
			if (!key)
				return (1);
			ft_strlcpy(key, argv[i], key_len + 1);
			value = equal_sign + 1;
			set_env_var(var, key, value);
			free(key);
		}
		// If no '=', just validate but don't set anything (as per bash behavior)
		i++;
	}
	return (0);
}

// unset with no options
int	builtin_unset(t_mini *var, char **argv)
{
	int	i;

	if (!argv[1])
		return (0);
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			write(2, "minishell: unset: `", 19);
			write(2, argv[i], ft_strlen(argv[i]));
			write(2, "': not a valid identifier\n", 26);
			i++;
			continue;
		}
		unset_env_var(var, argv[i]);
		i++;
	}
	return (0);
}
