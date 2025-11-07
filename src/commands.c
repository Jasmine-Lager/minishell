/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:29:08 by ksevciko          #+#    #+#             */
/*   Updated: 2025/11/08 00:42:54 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks for SIGINT (g_signal == 130) and updates exit code.
 * * Resets g_signal to 0 if it was set.
 *
 * @param var The main struct.
 */
static void	check_and_reset_signal(t_mini *var)
{
	if (g_signal == 130)
	{
		var->exit_code = 130;
		g_signal = 0;
	}
}

/**
 * @brief Manages the full continuation-handling process.
 * * Calls read_continuation, updates var->line, and re-parses.
 *
 * @param var The main struct.
 * @return 1 on success, 0 on failure (e.g., EOF or parse error).
 */
int	handle_line_continuation(t_mini *var)
{
	char	*complete_line;
	int		parse_status;

	complete_line = read_continuation(var, ft_strdup(var->line));
	if (!complete_line)
		return (0);
	free_tokens(var);
	var->tokens = NULL;
	var->needs_continuation = 0;
	var->nbr_pipes = 0;
	free(var->line);
	var->line = complete_line;
	parse_status = parse(var);
	if (!parse_status || !var->tokens)
		return (0);
	return (1);
}

// Main function to parse, handle continuations, and execute a command.
void	handle_command(t_mini *var)
{
	int	parse_status;
	int	exec_status;

	var->needs_continuation = 0;
	parse_status = parse(var);
	if (!var->needs_continuation)
		add_history(var->line);
	if (!parse_status || !var->tokens)
	{
		check_and_reset_signal(var);
		return ;
	}
	if (var->needs_continuation)
	{
		if (!handle_line_continuation(var))
			return ;
		add_history(var->line);
	}
	exec_status = execute_cmds(var);
	if (!exec_status)
		var->exit_code = 1;
	check_and_reset_signal(var);
}

// static void	print_tokens(t_token *tokens)
// {
// 	while (tokens)
// 	{
// 		ft_printf("%s (%i)\n", tokens->content, tokens->type);
// 		tokens = tokens->next;
// 	}
// }