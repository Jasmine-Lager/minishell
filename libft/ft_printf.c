/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:49:40 by ksevciko          #+#    #+#             */
/*   Updated: 2024/11/03 20:00:34 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	process_ptr(void *ptr, int fd)
{
	int	count;

	count = 0;
	if (!ptr)
	{
		count += write(fd, "(nil)", 5);
		return (count);
	}
	count += write(fd, "0x", 2);
	count += ft_putptr_base_fd((uintptr_t)ptr, "0123456789abcdef", 16, fd);
	return (count);
}

int	process_str(char *str, int fd)
{
	int	count;

	count = 0;
	if (!str)
	{
		count += write(fd, "(null)", 6);
		return (count);
	}
	count += ft_putstr_fd(str, fd);
	return (count);
}

int	process_args(char type, va_list args, int fd)
{
	int	count;

	count = 0;
	if (type == 'c')
		count += ft_putchar_fd((char)va_arg(args, int), fd);
	else if (type == 's')
		count += process_str(va_arg(args, char *), fd);
	else if (type == 'p')
		count += process_ptr(va_arg(args, void *), fd);
	else if (type == 'd' || type == 'i')
		count += ft_putnbr_fd(va_arg(args, int), fd);
	else if (type == 'u')
		count += ft_putunsigned_fd(va_arg(args, unsigned int), fd);
	else if (type == 'x')
		count += ft_putnbr_base_fd(va_arg(args, int),
				"0123456789abcdef", 16, fd);
	else if (type == 'X')
		count += ft_putnbr_base_fd(va_arg(args, int),
				"0123456789ABCDEF", 16, fd);
	else if (type == '%')
		count += write(fd, "%", 1);
	return (count);
}

int	format_result(const char *format, va_list args, int fd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			count += process_args(format[++i], args, fd);
		}
		else
		{
			count += write(fd, &format[i], 1);
		}
		i++;
	}
	return (count);
}

int	ft_printf(const char *format, ...)
{
	int		n;
	va_list	args;
	int		fd;

	fd = 1;
	va_start(args, format);
	n = format_result(format, args, fd);
	if (n == -1)
		return (-1);
	va_end(args);
	return (n);
}
