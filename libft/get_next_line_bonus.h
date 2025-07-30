/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksevciko <ksevciko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:00:54 by ksevciko          #+#    #+#             */
/*   Updated: 2025/03/15 22:16:53 by ksevciko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <limits.h>

# ifndef MY_OPEN_MAX
#  define MY_OPEN_MAX 4096
# endif

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 128
# endif

char	*get_next_line(int fd);

// static int		read_until_newline(int fd, char **buffer, size_t *i,
// 			ssize_t *buff_size);
// static char	*start_reading(int fd, char *buffer,
//			size_t *i, ssize_t *buff_size);
// static char	*copy_one_line(char *buffer, size_t i, ssize_t buff_size);

char	*my_simplified_realloc(char *buffer, size_t size);
size_t	find_index_of_newline(char *buffer);
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);

#endif
