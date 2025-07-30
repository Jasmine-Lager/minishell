/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_base_to_uint.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 22:11:48 by ksevciko          #+#    #+#             */
/*   Updated: 2025/03/15 23:46:48 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

uint	ft_base_to_uint(char *nbr, char *base, size_t base_nbr)
{
	uint	result;
	int		found;
	int		i;

	result = 0;
	while (nbr && base && *nbr && base_nbr == ft_strlen(base))
	{
		found = 0;
		i = 0;
		while (base[i])
		{
			if (*nbr == base[i])
			{
				found = 1;
				result = result * base_nbr + i;
				break ;
			}
			i++;
		}
		if (!found)
			return (result);
		nbr++;
	}
	return (result);
}
