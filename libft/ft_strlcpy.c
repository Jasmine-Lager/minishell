/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 20:43:10 by ksevciko          #+#    #+#             */
/*   Updated: 2025/09/28 01:50:54 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	while (i < (size - 1) && src && src[i] != '\0' && size != 0)
	{
		dst[i] = src[i];
		i++;
	}
	if (i < size)
		dst[i] = '\0';
	while (src && src[i])
	{
		i++;
	}
	return (i);
}

//no crash version: does not segfault
//even when null is sent, whether as dst, or as src:
// size_t	ft_strlcpy(char *dst, const char *src, size_t size) 
// {
// 	size_t	i;

// 	i = 0;
// 	while (src && i + 1 < size && src[i] != '\0')
// 	{
// 		dst[i] = src[i];
// 		i++;
// 	}
// 	if (i < size && dst)
// 		dst[i] = '\0';
// 	while (src && src[i])
// 	{
// 		i++;
// 	}
// 	return (i);
// }
