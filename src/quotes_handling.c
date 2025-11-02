/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:39:20 by jasminelage       #+#    #+#             */
/*   Updated: 2025/11/02 18:21:56 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Set quote info based on section counts
t_quote_info	set_quote_behavior(t_quote_counts counts)
{
	t_quote_info	info;

	info.mixed_quotes = 0;
	info.needs_processing = 0;
	info.dominant_type = NONE;
	if (is_pure_single(counts))
	{
		info.dominant_type = SINGLE;
		info.needs_processing = 1;
	}
	else if (is_pure_double(counts))
	{
		info.dominant_type = DOUBLE;
		info.needs_processing = 1;
	}
	else if (counts.single_sections > 0 || counts.double_sections > 0)
	{
		info.mixed_quotes = 1;
		info.needs_processing = 1;
		info.dominant_type = get_dominant_type(counts);
	}
	return (info);
}

// Count quote sections in token content
t_quote_counts	count_quote_sections(char *content)
{
	t_quote_counts	counts;
	int				i;
	int				in_single;
	int				in_double;

	counts.single_sections = 0;
	counts.double_sections = 0;
	counts.unquoted_sections = 0;
	i = 0;
	in_single = 0;
	in_double = 0;
	while (content[i])
	{
		if (content[i] == '\'' && !in_double)
			handle_single_quote(&in_single, &counts);
		else if (content[i] == '"' && !in_single)
			handle_double_quote(&in_double, &counts);
		else if (!in_single && !in_double && content[i] != '\''
			&& content[i] != '"')
			if (i == 0 || (content[i - 1] == '\'' || content[i - 1] == '"'))
				counts.unquoted_sections++;
		i++;
	}
	return (counts);
}

t_quote_info	analyze_token_quotes_detailed(char *content)
{
	t_quote_counts	counts;
	t_quote_info	info;

	counts = count_quote_sections(content);
	info = set_quote_behavior(counts);
	return (info);
}
