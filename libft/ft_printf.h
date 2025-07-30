/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 17:56:27 by ksevciko          #+#    #+#             */
/*   Updated: 2025/03/15 22:16:59 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stddef.h>
# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include <stdarg.h>
# include <stdint.h>
# include "libft.h"

int	ft_printf(const char *format, ...);

int	ft_putnbr_base_fd(unsigned int nbr, char *base, int base_nbr, int fd);
int	ft_putptr_base_fd(uintptr_t nbr, char *base, int base_nbr, int fd);
int	ft_putunsigned_fd(unsigned int n, int fd);

#endif
