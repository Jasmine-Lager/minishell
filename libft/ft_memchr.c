/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 17:14:51 by ksevciko          #+#    #+#             */
/*   Updated: 2024/09/17 18:38:23 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	size_t			i;
	void			*result;

	str = (unsigned char *)s;
	i = 0;
	while (i + 1 < n && str[i] != (unsigned char)c)
	{
		i++;
	}
	if (n > 0 && str[i] == (unsigned char)c)
	{
		result = (void *)&str[i];
		return (result);
	}
	return (NULL);
}
