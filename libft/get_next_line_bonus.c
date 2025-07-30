/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 19:28:06 by ksevciko          #+#    #+#             */
/*   Updated: 2025/03/15 22:19:09 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static int	read_until_newline(int fd, char **buffer,
		size_t *i, ssize_t *buff_size)
{
	ssize_t	buff_i;
	ssize_t	read_error;

	buff_i = 0;
	while (*i == 0 && (*buffer)[0] != '\n')
	{
		if (buff_i == 0)
			buff_i = ft_strlen(*buffer);
		if ((*buff_size) - buff_i - 1 < BUFFER_SIZE)
		{
			*buffer = my_simplified_realloc(*buffer, (*buff_size) * 2);
			if (!(*buffer))
				return (0);
			*buff_size = 2 * (*buff_size);
		}
		read_error = read(fd, (*buffer) + buff_i, BUFFER_SIZE);
		if (read_error < 0)
			return (0);
		buff_i = buff_i + read_error;
		(*buffer)[buff_i] = '\0';
		*i = find_index_of_newline(*buffer);
		if (read_error < BUFFER_SIZE)
			return (1);
	}
	return (1);
}

static char	*start_reading(int fd, char *buffer, size_t *i, ssize_t *buff_size)
{
	int	read_error;

	if (!buffer)
	{
		buffer = (char *)malloc(2 * BUFFER_SIZE);
		if (!buffer)
			return (NULL);
		buffer[0] = '\0';
		*buff_size = 2 * BUFFER_SIZE;
	}
	*i = find_index_of_newline(buffer);
	if (*buff_size == 0)
		*buff_size = ft_strlen(buffer);
	if (*buff_size < 2 * BUFFER_SIZE)
		*buff_size = 2 * BUFFER_SIZE;
	if (*i != 0 || buffer[0] == '\n')
		return (buffer);
	read_error = read_until_newline(fd, &buffer, i, buff_size);
	if (read_error == 0)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	return (buffer);
}

static char	*copy_one_line(char *buffer, size_t i, ssize_t buff_size)
{
	char	*result;

	if (i == 0 && buffer[0] != '\n')
	{
		i = ft_strlen(buffer);
		if (i == 0)
			return (NULL);
		result = (char *)malloc(i + 1);
		if (!result)
			return (NULL);
		ft_strlcpy(result, buffer, i + 1);
		ft_strlcpy(buffer, buffer + i, (size_t)buff_size);
		return (result);
	}
	result = (char *)malloc(i + 2);
	if (!result)
		return (NULL);
	ft_strlcpy(result, buffer, i + 2);
	ft_strlcpy(buffer, buffer + i + 1, buff_size);
	return (result);
}

char	*get_next_line(int fd)
{
	static char	*buffer[MY_OPEN_MAX];
	size_t		i;
	char		*result;
	ssize_t		buff_size;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buff_size = 0;
	i = 0;
	buffer[fd] = start_reading(fd, buffer[fd], &i, &buff_size);
	if (!(buffer[fd]))
		return (NULL);
	result = copy_one_line(buffer[fd], i, buff_size);
	if (!result)
	{
		free(buffer[fd]);
		buffer[fd] = NULL;
		return (NULL);
	}
	if (*buffer[fd] == '\0')
	{
		free(buffer[fd]);
		buffer[fd] = NULL;
	}
	return (result);
}

// #include <fcntl.h>

// int	main(void)
// {
// 	char	*line;
// 	int		fd;

// 	fd = open("1char.txt", O_RDWR);
// 	line = "l";
// 	while (line)
// 	{
// 		line = get_next_line(fd);
// 		write(1, line, ft_strlen(line));	
// 		free(line);
// 	}
// 	close(fd);
// 	return (0);
// }

// int	main(void)
// {
// 	char	*line;
// 	int		fd;

// 	fd = open("1char.txt", O_RDWR);
// 	line = get_next_line(fd);

// 	printf("1 %s", line);
// 	free(line);

// 	line = get_next_line(fd);
// 	printf("2 %s", line);
// 	free(line);

// 	close(fd);
// 	return (0);
// }
