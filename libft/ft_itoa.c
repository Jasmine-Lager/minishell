/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:58:16 by ksevciko          #+#    #+#             */
/*   Updated: 2024/09/20 15:47:01 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*create_str(int n, size_t len, int negative, long tens)
{
	char	*str;
	size_t	i;

	i = 0;
	str = (char *)malloc((len + negative + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	if (negative == 1)
		str[i++] = '-';
	while (tens > 0)
	{
		str[i++] = '0' + (n / tens) % 10;
		tens /= 10;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	long	tens;
	size_t	len;
	int		negative;

	tens = 1;
	len = 1;
	negative = 0;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n < 0)
	{
		n = -n;
		negative = 1;
	}
	while (n / (tens * 10) != 0)
	{
		len++;
		tens *= 10;
	}
	return (create_str(n, len, negative, tens));
}
