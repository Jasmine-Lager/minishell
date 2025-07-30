/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:14:44 by ksevciko          #+#    #+#             */
/*   Updated: 2024/09/22 21:31:28 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	string_counter(char const *s, char c)
{
	size_t	i;
	int		in_string;
	size_t	string_count;

	if (s == NULL)
		return (0);
	i = 0;
	in_string = 0;
	string_count = 0;
	while (s[i])
	{
		if (in_string == 1 && s[i] == c)
			string_count += 1;
		if (s[i] == c)
			in_string = 0;
		else
			in_string = 1;
		i++;
	}
	if (in_string == 1)
		string_count += 1;
	return (string_count);
}

static size_t	len_one_str(char const *s, char c)
{
	size_t	i;

	if (s == NULL)
		return (0);
	i = 0;
	while (s[i] != c && s[i])
	{
		i++;
	}
	return (i);
}

static int	allocate_for_str(char **result, int str_len, size_t i)
{
	size_t	j;

	result[i] = (char *)malloc((str_len + 1) * sizeof(char));
	if (result[i] == NULL)
	{
		j = 0;
		while (j < i)
			free(result[j++]);
		free(result);
		result = NULL;
		return (-1);
	}
	return (str_len);
}

char	**ft_split(char const *s, char c)
{
	size_t	string_count;
	char	**result;
	size_t	i;
	size_t	j;
	int		str_len;

	string_count = string_counter(s, c);
	result = (char **)malloc((string_count + 1) * sizeof(char *));
	if (result == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (i < string_count && s != NULL)
	{
		while (s[j] == c)
			j++;
		str_len = len_one_str(&s[j], c);
		str_len = allocate_for_str(result, str_len, i);
		if (str_len == -1)
			return (NULL);
		ft_strlcpy(result[i++], &s[j], str_len + 1);
		j += str_len;
	}
	result[i] = NULL;
	return (result);
}
