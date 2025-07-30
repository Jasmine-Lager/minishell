/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr_base_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 13:45:48 by ksevciko          #+#    #+#             */
/*   Updated: 2025/03/15 22:19:39 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// long int	base_number(char *base)
// {
// 	int	base_nbr1;
// 	int	base_nbr2;

// 	base_nbr1 = 0;
// 	if (base[0] == '\0')
// 		return (0);
// 	while (base[base_nbr1] != '\0')
// 	{
// 		if (base[base_nbr1] == '-' || base[base_nbr1] == '+')
// 			return (0);
// 		base_nbr2 = 0;
// 		while (base_nbr2 < base_nbr1)
// 		{
// 			if (base[base_nbr1] == base[base_nbr2])
// 				return (0);
// 			base_nbr2++;
// 		}
// 		base_nbr1++;
// 	}
// 	return (base_nbr1);
// }

static long int	ft_pow(int nbr, int pwr) //pwr must be positive or 0 and int
{
	int						i;
	unsigned long long int	n;

	if (pwr == 0)
		return (1);
	i = 1;
	n = nbr;
	while (i < pwr)
	{
		n = n * nbr;
		i++;
	}
	return (n);
}

static int	highest_pwr(uintptr_t n, int base_nbr)
{
	int	pwr;

	pwr = 0;
	while (n / base_nbr != 0)
	{
		n = n / base_nbr;
		pwr++;
	}
	return (pwr);
}

int	ft_putptr_base_fd(uintptr_t nbr, char *base, int base_nbr, int fd)
{
	int						pwr;
	unsigned long long int	base_to_pwr;
	int						count;

	count = 0;
	pwr = highest_pwr(nbr, base_nbr);
	while (pwr >= 0)
	{
		base_to_pwr = ft_pow(base_nbr, pwr--);
		count += write(fd, &base[nbr / base_to_pwr], 1);
		nbr = nbr - base_to_pwr * (nbr / base_to_pwr);
	}
	return (count);
}

// long int	base_nbr;
// base_nbr = base_number(base);
// if (base_nbr < 2)
// 	return (-1);

// #include <limits.h>
// int	main(void)
// {
// 	int		nbr;
// 	char	*base = "01";
// 	nbr = -90;
// 	ft_putnbr_base(INT_MIN, base);
// }
