/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:38:26 by ksevciko          #+#    #+#             */
/*   Updated: 2025/02/27 17:23:49 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	find_index_of_newline(char *buffer)
{
	size_t	i;

	i = 0;
	if (!buffer)
		return (0);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		return (i);
	return (0);
}

char	*my_simplified_realloc(char *buffer, size_t size)
{
	char	*result;

	result = (char *)malloc(size);
	if (!result)
	{
		free(buffer);
		return (NULL);
	}
	ft_strlcpy(result, buffer, size);
	free(buffer);
	return (result);
}
//if i want to use get next line without libft, 
//i need these, otherwise they are useless
// size_t	ft_strlen(const char *s)
// {
// 	size_t	i;

// 	i = 0;
// 	while (s && s[i])
// 	{
// 		i++;
// 	}
// 	return (i);
// }

// size_t	ft_strlcpy(char *dst, const char *src, size_t size)
// {
// 	size_t	i;

// 	i = 0;
// 	while (src && i + 1 < size && src[i] != '\0' && dst)
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
