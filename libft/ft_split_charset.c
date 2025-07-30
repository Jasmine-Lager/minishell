/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 13:30:52 by ksevciko          #+#    #+#             */
/*   Updated: 2024/08/23 12:30:56 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	my_special_strlen(char *str, int *is_sep, int normal)
{
	int	i;

	if (normal == 1)
	{
		i = 0;
		while (str && str[i] != '\0')
		{
			i++;
		}
		return (i);
	}
	i = 0;
	while (is_sep && is_sep[i] != 1 && is_sep[i] != 42)
	{
		i++;
	}
	return (i);
}

static int	*is_sep_in_str(char *str, char *charset)
{
	int	i;
	int	*is_sep;
	int	j;
	int	blah[1];

	i = 0;
	blah[0] = 42;
	is_sep = (int *)malloc((my_special_strlen(str, blah, 1) + 1) * sizeof(int));
	if (!is_sep)
		return (NULL);
	while (str && str[i])
	{
		is_sep[i] = 0;
		j = 0;
		while (charset && charset[j++])
		{
			if (str[i] == charset[j])
				is_sep[i] = 1;
		}
		i++;
	}
	is_sep[i] = 42;
	return (is_sep);
}

static int	string_counter(char *str, int *is_sep)
{
	int	i;
	int	in_string;
	int	string_count;

	i = 0;
	in_string = 0;
	string_count = 0;
	while (str && str[i] && is_sep && is_sep[i])
	{
		if (in_string == 1 && is_sep[i] == 1)
			string_count += 1;
		if (is_sep[i] == 1)
			in_string = 0;
		else
			in_string = 1;
		i++;
	}
	if (in_string == 1)
		string_count += 1;
	return (string_count);
}

static int	my_special_strcpy(char *src, char *dest, int *is_sep, int j)
{
	int	i;

	i = 0;
	while (src && src[j + i] && is_sep && dest && is_sep[j + i] == 0)
	{
		dest[i] = src[j + i];
		i++;
	}
	dest[i] = '\0';
	return (i);
}

char	**ft_split_charset(char *str, char *charset)
{
	int		string_count;
	char	**strs;
	int		i;
	int		j;
	int		*is_sep;

	is_sep = is_sep_in_str(str, charset);
	string_count = string_counter(str, is_sep);
	strs = (char **)malloc((string_count + 1) * sizeof(char *));
	if (!strs || !is_sep)
		return (NULL);
	i = 0;
	j = 0;
	while (i < string_count)
	{
		while (is_sep[j])
			j++;
		strs[i] = (char *)malloc(my_special_strlen(str, &is_sep[j], 0) + 1);
		j += my_special_strcpy(str, strs[i], is_sep, j);
		i++;
	}
	free(is_sep);
	strs[i] = NULL;
	return (strs);
}

// #include <unistd.h>

// void	ft_putstr(char *str)
// {
// 	while (*str != '\0')
// 	{
// 		write(1, str, 1);
// 		str = str + 1;
// 	}
// }

// int	main(int argc, char **argv)
// {
// 	int		i;
// 	char	**lst;
// 	char	*sep = "+.:;,-";

// 	i = 0;
// 	argv++;
// 	lst = malloc(555);
// 	if (argc >= 2)
// 		lst = ft_split(argv[0], sep);
// 	ft_putstr(*lst);
// 	return (0);
// }
