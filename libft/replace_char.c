/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 19:06:51 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/02 19:06:51 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*replace_char(char *str, char old, char new)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == old)
			str[i] = new;
		i++;
	}
	return (str);
}
