/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:15:48 by ksevciko          #+#    #+#             */
/*   Updated: 2024/11/01 19:38:08 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_fd(int n, int fd)
{
	long int	tens;
	char		c;
	long int	nbr;
	int			count;

	tens = 1;
	nbr = n;
	count = 0;
	if (nbr < 0)
	{
		nbr = -nbr;
		count += write(fd, "-", 1);
	}
	while (nbr / (tens * 10) != 0)
		tens *= 10;
	while (tens > 0)
	{
		c = '0' + (nbr / tens) % 10;
		count += write(fd, &c, 1);
		tens /= 10;
	}
	return (count);
}
