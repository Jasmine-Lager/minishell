/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_handling_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:31:14 by jasminelage       #+#    #+#             */
/*   Updated: 2025/09/18 13:43:04 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to check if quotes are pure single type
int is_pure_single(t_quote_counts counts)
{
	return (counts.single_sections == 1 &&
			counts.double_sections == 0 && counts.unquoted_sections == 0);
}

// Helper function to check if quotes are pure double type
int is_pure_double(t_quote_counts counts)
{
	return (counts.double_sections == 1 &&
			counts.single_sections == 0 && counts.unquoted_sections == 0);
}

// Determine dominant quote type for mixed quotes
t_quotes get_dominant_type(t_quote_counts counts)
{
	if (counts.double_sections > counts.single_sections)
		return (DOUBLE);
	if (counts.single_sections > 0)
		return (SINGLE);
	return (NONE);
}

// Helper function to handle single quote logic
void handle_single_quote(int *in_single, t_quote_counts *counts)
{
	if (!(*in_single))
	{
		*in_single = 1;
		counts->single_sections++;
	}
	else
		*in_single = 0;
}

// Helper function to handle double quote logic
void handle_double_quote(int *in_double, t_quote_counts *counts)
{
	if (!(*in_double))
	{
		*in_double = 1;
		counts->double_sections++;
	}
	else
		*in_double = 0;
}
