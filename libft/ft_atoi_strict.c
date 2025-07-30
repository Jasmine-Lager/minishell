/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_strict.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:29:43 by ksevciko          #+#    #+#             */
/*   Updated: 2025/04/25 15:29:43 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi_strict(const char *nptr)
{
	unsigned int	result;
	int				sign;

	result = 0;
	sign = 1;
	if (!nptr)
		return (0);
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = sign * (-1);
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		result = result * 10 + (*nptr - '0');
		nptr++;
		if ((result > INT_MAX / 10 && *nptr != '\0') || (result - 1 > INT_MAX))
			return (0);
		if ((result > INT_MAX && sign == 1) || (int)result * sign < INT_MIN)
			return (0);
	}
	if (*nptr != '\0')
		return (0);
	return (result * sign);
}
