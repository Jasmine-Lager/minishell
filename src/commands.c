/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:29:08 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/04 15:11:28 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_continuation(t_mini *var, char *current_line)
{
	char	*new_line;
	char	*combined;
	char	*temp;

	while (var->needs_continuation)
	{
		new_line = readline("> "); 
		if (!new_line)
		{
			write(2, "minishell: unexpected EOF while looking for matching pipe\n", 59);
			free(current_line);
			return (NULL);
		}
		if (!*new_line)
		{
			free(new_line);
			continue;
		}
		temp = ft_strjoin(current_line, " ");
		if (!temp)
			return (NULL);
		combined = ft_strjoin(temp, new_line);
		free(temp);
		free(new_line);
		free(current_line);
		if (!combined)
			return (NULL);
		current_line = combined;
		free_tokens(var);
		var->tokens = NULL;
		var->needs_continuation = 0;
		var->nbr_pipes = 0;
		temp = var->line;
		var->line = current_line;
		if (!parse(var))
		{
			var->line = temp;
			free(current_line);
			return (NULL);
		}
		var->line = temp;
	}
	return (current_line);
}

void	handle_command(t_mini *var)
{
	char	*complete_line;

	var->needs_continuation = 0;
	if (!parse(var) || !var->tokens)
	{
		if (g_signal == 130)
		{
			var->exit_code = 130;
			g_signal = 0;
		}
		return ;
	}
	if (var->needs_continuation)
	{
		complete_line = read_continuation(var, ft_strdup(var->line));
		if (!complete_line)
			return ;
		free_tokens(var);
		var->tokens = NULL;
		var->needs_continuation = 0;
		var->nbr_pipes = 0;
		free(var->line);
		var->line = complete_line;
		if (!parse(var) || !var->tokens)
			return ;
	}
	add_history(var->line);
	if (!execute_cmds(var))
	{
		if (g_signal == 130)
		{
			var->exit_code = 130;
			g_signal = 0;
		}
		return ;
	}
	if (g_signal == 130)
	{
		var->exit_code = 130;
		g_signal = 0;
	}
}

// static void	print_tokens(t_token *tokens)
// {
// 	while (tokens)
// 	{
// 		ft_printf("%s (%i)\n", tokens->content, tokens->type);
// 		tokens = tokens->next;
// 	}
// }