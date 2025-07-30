/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_max.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 18:13:28 by ksevciko          #+#    #+#             */
/*   Updated: 2025/03/15 19:09:30 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_max2(int x, int y)
{
	if (x >= y)
		return (x);
	return (y);
}

int	ft_max3(int x, int y, int z)
{
	if (x >= y)
	{
		if (x >= z)
			return (x);
		return (z);
	}
	if (y >= z)
		return (y);
	return (z);
}

int	ft_min2(int x, int y)
{
	if (x < y)
		return (x);
	return (y);
}
