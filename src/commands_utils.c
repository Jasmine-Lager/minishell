/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:46:13 by jasminelage       #+#    #+#             */
/*   Updated: 2025/11/05 14:24:12 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles readline EOF (NULL return) during line continuation.
 * * @param current_line The line buffer to be freed.
 * @return Always returns NULL.
 */
static char	*handle_continuation_eof(char *current_line)
{
	write(2, "minishell: unexpected EOF while looking for matching pipe\n", 59);
	free(current_line);
	return (NULL);
}

/**
 * @brief Appends new_line to current_line, separated by a space.
 * * Manages all memory for old/intermediate strings.
 * Frees current_line and new_line.
 *
 * @param current_line The original line.
 * @param new_line The new line to append.
 * @return A new, combined string, or NULL on allocation failure.
 */
static char	*append_line(char *current_line, char *new_line)
{
	char	*temp;
	char	*combined;

	temp = ft_strjoin(current_line, " ");
	if (!temp)
	{
		free(current_line);
		free(new_line);
		return (NULL);
	}
	combined = ft_strjoin(temp, new_line);
	free(temp);
	free(current_line);
	free(new_line);
	if (!combined)
		return (NULL);
	return (combined);
}

/**
 * @brief Resets parser state and re-parses the provided line.
 * * This function temporarily points var->line to current_line for
 * parsing, then restores the original var->line pointer.
 *
 * @param var The main struct.
 * @param current_line The new, complete line to be parsed.
 * @return 1 on successful parse, 0 on failure.
 */
static int	reparse_line(t_mini *var, char *current_line)
{
	char	*original_line_ptr;
	int		parse_status;

	free_tokens(var);
	var->tokens = NULL;
	var->needs_continuation = 0;
	var->nbr_pipes = 0;
	original_line_ptr = var->line;
	var->line = current_line;
	parse_status = parse(var);
	var->line = original_line_ptr;
	if (!parse_status)
		return (0);
	return (1);
}

// Reads continued input lines until the parser is satisfied.
char	*read_continuation(t_mini *var, char *current_line)
{
	char	*new_line;

	while (var->needs_continuation)
	{
		new_line = readline("> ");
		if (!new_line)
			return (handle_continuation_eof(current_line));
		if (!*new_line)
		{
			free(new_line);
			continue ;
		}
		current_line = append_line(current_line, new_line);
		if (!current_line)
			return (NULL);
		if (!reparse_line(var, current_line))
		{
			free(current_line);
			return (NULL);
		}
	}
	return (current_line);
}
