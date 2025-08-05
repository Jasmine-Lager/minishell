/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlager <jlager@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:24 by jlager            #+#    #+#             */
/*   Updated: 2025/08/05 16:33:52 by jlager           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	return_error(const char *error_msg)
{
	ft_printf(BOLD RED "❗️%s❗️\n" RESET, error_msg);
	// or we can do the same with perror();
	exit(EXIT_FAILURE);
}