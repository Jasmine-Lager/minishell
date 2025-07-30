/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_after_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:08:00 by ksevciko          #+#    #+#             */
/*   Updated: 2025/04/26 13:08:00 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_arr(char **split_line)
{
	int	i;

	i = 0;
	while (split_line && split_line[i])
	{
		free(split_line[i]);
		split_line[i] = NULL;
		i++;
	}
	free(split_line);
	split_line = NULL;
}
