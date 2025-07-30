/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 20:50:55 by ksevciko          #+#    #+#             */
/*   Updated: 2024/09/19 17:06:26 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	find_start(char const *s1, char const *set)
{
	size_t	start;
	size_t	i;

	start = 0;
	while (s1[start])
	{
		i = 0;
		while (set[i])
		{
			if (s1[start] == set[i])
			{
				start++;
				break ;
			}
			i++;
		}
		if (!set[i])
			break ;
	}
	return (start);
}

static size_t	find_end(char const *s1, char const *set, size_t start)
{
	size_t	end;
	size_t	i;

	end = ft_strlen(s1);
	while (end > start)
	{
		i = 0;
		while (set[i])
		{
			if (s1[end - 1] == set[i])
			{
				end--;
				break ;
			}
			i++;
		}
		if (!set[i])
			break ;
	}
	return (end);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*result;
	size_t	start;
	size_t	end;

	start = find_start(s1, set);
	end = find_end(s1, set, start);
	result = (char *)malloc((end - start + 1) * sizeof(char));
	if (result == NULL)
		return (NULL);
	ft_strlcpy(result, s1 + start, end - start + 1);
	return (result);
}
