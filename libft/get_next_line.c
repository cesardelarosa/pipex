/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-la-r <cde-la-r@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 04:50:34 by cde-la-r          #+#    #+#             */
/*   Updated: 2023/11/03 14:08:58 by cde-la-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
#include "libft.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

#ifndef OPEN_MAX
# define OPEN_MAX 1024
#endif

static char	*read_buffer(int fd, char *remainder)
{
	char	buffer[BUFFER_SIZE + 1];
	ssize_t	bytes_read;
	char	*temp;

	bytes_read = 1;
	while (!ft_strchr(remainder, '\n') && bytes_read != 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(remainder);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(remainder, buffer);
		if (!temp)
		{
			free(remainder);
			return (NULL);
		}
		free(remainder);
		remainder = temp;
	}
	return (remainder);
}

static char	*create_line(char *remainder, size_t len)
{
	char	*line;
	size_t	i;

	line = (char *)malloc(len + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (i < len)
	{
		line[i] = remainder[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*update_remainder(char *remainder, size_t start)
{
	char	*new_remainder;
	size_t	len;
	size_t	i;

	len = ft_strlen(remainder + start);
	if (len == 0)
	{
		free(remainder);
		return (NULL);
	}
	new_remainder = (char *)malloc(len + 1);
	if (!new_remainder)
	{
		free(remainder);
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		new_remainder[i] = remainder[start + i];
		i++;
	}
	new_remainder[i] = '\0';
	free(remainder);
	return (new_remainder);
}

static char	*extract_line(char **remainder)
{
	char	*newline_pos;
	char	*line;
	size_t	len;

	newline_pos = ft_strchr(*remainder, '\n');
	if (newline_pos)
	{
		len = newline_pos - *remainder + 1;
		line = create_line(*remainder, len);
		if (!line)
			return (NULL);
		*remainder = update_remainder(*remainder, len);
	}
	else
	{
		len = ft_strlen(*remainder);
		line = create_line(*remainder, len);
		if (!line)
			return (NULL);
		free(*remainder);
		*remainder = NULL;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*remainder[OPEN_MAX];
	char		*line;

	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	if (!remainder[fd])
	{
		remainder[fd] = ft_strdup("");
		if (!remainder[fd])
			return (NULL);
	}
	remainder[fd] = read_buffer(fd, remainder[fd]);
	if (!remainder[fd])
		return (NULL);
	line = extract_line(&remainder[fd]);
	return (line);
}
