/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 22:25:57 by ksevciko          #+#    #+#             */
/*   Updated: 2025/07/21 15:42:00 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*result;
	size_t	i;
	size_t	total_size;

	total_size = nmemb * size;
	if (nmemb == 0 || size == 0 || total_size / nmemb != size)
		return (NULL);
	result = (char *)malloc(total_size);
	if (result == NULL)
		return (NULL);
	i = 0;
	while (i < total_size)
	{
		result[i] = '\0';
		i++;
	}
	return ((void *)result);
}
