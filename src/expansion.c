/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:57 by jlager            #+#    #+#             */
/*   Updated: 2025/09/16 15:46:32 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// The expander handles variable substitution, tilde expansion,
// and quote removal before execution. This stage processes $HOME, $?,
// and environment variables.

// Expansion Tasks (Variable Processing):
// Environment variable substitution ($VAR)  - use find_env_var from initialize_var.c
//DO NOT expand anything in delimiter, do not do variable substitution for delimiter
// DO mark if each token was quoted
// Exit status expansion ($?)
// Home directory expansion (~)
// Quote removal after expansion

void	expand(char **str, bool double_quoted)
{
	int	i;

	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '$')
		{
			//expand that
		}
		else if ((*str)[i] == '"')
		{
			//handle "
		}
		else if (!double_quoted && (*str)[i] == 39)
		{
			//handle '
		}
	}
}
