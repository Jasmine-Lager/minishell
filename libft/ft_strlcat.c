/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:42:40 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/02 14:22:31 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	len;
	size_t	i;

	len = 0;
	while (size != 0 && dst[len] != '\0' && len < size)
		len++;
	i = 0;
	while (size != 0 && src && src[i] != '\0' && len + i + 1 < size)
	{
		dst[len + i] = src[i];
		i++;
	}
	if (len + i < size)
		dst[len + i] = '\0';
	while (src && src[i])
		i++;
	return (len + i);
}
