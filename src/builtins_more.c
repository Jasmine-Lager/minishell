/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_more.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:10:22 by jasminelage       #+#    #+#             */
/*   Updated: 2025/11/11 13:56:04 by jlager           ###   ########.fr       */
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
	while (str[i] && str[i] != '=' && str[i] != '+')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// Helper function for export
static int	export_single_arg(t_mini *var, char *arg)
{
	char	*equal_sign;
	char	*key;
	char	*value;
	int		key_len;

	if (!is_valid_identifier(arg))
		return (export_message(arg));
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		key_len = equal_sign - (*(equal_sign - 1) == '+') - arg;
		key = malloc(key_len + 1);
		if (!key || !ft_strlcpy(key, arg, key_len + 1))
			return (1);
		if (*(equal_sign - 1) == '+')
			value = ft_strjoin(find_env_var(var->envp, key), equal_sign + 1);
		else
			value = ft_strdup(equal_sign + 1);
		if (!value)
			return (free(key), 1);
		set_env_var(var, key, value);
		free(key);
		free(value);
	}
	return (0);
}

// export with no options
int	builtin_export(t_mini *var, char **argv)
{
	int	i;
	int	ret;

	if (!argv[1])
	{
		print_export(var);
		return (0);
	}
	i = 1;
	ret = 0;
	while (argv[i])
	{
		if (export_single_arg(var, argv[i]) == 1)
			ret = 1;
		i++;
	}
	return (ret);
}

// unset with no options
int	builtin_unset(t_mini *var, char **argv)
{
	int	i;
	int	ret;

	if (!argv[1])
		return (0);
	i = 1;
	ret = 0;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]) || ft_strchr(argv[i], '='))
		{
			write(2, "minishell: unset: `", 19);
			write(2, argv[i], ft_strlen(argv[i]));
			write(2, "': not a valid identifier\n", 26);
			ret = 1;
			i++;
			continue ;
		}
		unset_env_var(var, argv[i]);
		i++;
	}
	return (ret);
}
