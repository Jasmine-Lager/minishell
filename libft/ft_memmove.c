/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 20:24:52 by ksevciko          #+#    #+#             */
/*   Updated: 2024/09/16 20:42:18 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*dst;
	char	*s;

	dst = (char *)dest;
	s = (char *)src;
	if (dst < s)
	{
		i = 0;
		while (i < n)
		{
			dst[i] = s[i];
			i++;
		}
	}
	if (dst > s)
	{
		i = n;
		while (i > 0)
		{
			dst[i - 1] = s[i - 1];
			i--;
		}
	}
	return (dest);
}
