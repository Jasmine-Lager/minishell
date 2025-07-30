/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 20:00:18 by ksevciko          #+#    #+#             */
/*   Updated: 2024/09/17 15:31:12 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*dst;
	char	*s;

	i = 0;
	if (dest == NULL && src == NULL)
	{
		return (NULL);
	}
	dst = (char *)dest;
	s = (char *)src;
	while (i < n)
	{
		dst[i] = s[i];
		i++;
	}
	return (dest);
}
