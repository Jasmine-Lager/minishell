/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunsigned_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:15:48 by ksevciko          #+#    #+#             */
/*   Updated: 2024/11/01 20:11:49 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putunsigned_fd(unsigned int n, int fd)
{
	long unsigned int	tens;
	char				c;
	int					count;

	tens = 1;
	count = 0;
	while (n / (tens * 10) != 0)
		tens *= 10;
	while (tens > 0)
	{
		c = '0' + (n / tens) % 10;
		count += write(fd, &c, 1);
		tens /= 10;
	}
	return (count);
}
