/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_four.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 19:08:42 by ksevciko          #+#    #+#             */
/*   Updated: 2025/10/02 19:08:42 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_four(const char *s1, const char *s2,
			const char *s3, const char *s4)
{
	char	*result;
	char	*tmp;

	result = ft_strjoin(s1, s2);
	if (!result)
		return (NULL);
	tmp = ft_strjoin(result, s3);
	free(result);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, s4);
	free(tmp);
	if (!result)
		return (NULL);
	return (result);
}
