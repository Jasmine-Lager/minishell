/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:37:10 by ksevciko          #+#    #+#             */
/*   Updated: 2024/09/17 17:17:23 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*str;

	str = (char *)s;
	i = 0;
	while (str[i])
	{
		if (str[i] == (char)c)
		{
			str += i;
			i = 0;
		}
		i++;
	}
	if (str[i] == (char)c)
	{
		str += i;
	}
	if (*str == (char)c)
	{
		return (str);
	}
	return (NULL);
}
