/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:59:35 by sguan             #+#    #+#             */
/*   Updated: 2025/09/05 18:10:46 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4
# endif

static int	check_newline(char *buffer)
{
	int	i;

	i = 0;
	while (buffer[i])
	{
		if (buffer[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

static char	*extract_line(char **leftover, size_t newline_index)
{
	char	*line;
	char	*temp;

	line = ft_substr(*leftover, 0, newline_index + 1);
	if (!line)
		return (NULL);
	temp = ft_strdup(*leftover + newline_index + 1);
	free(*leftover);
	*leftover = temp;
	return (line);
}

static char	*update_leftover(char **leftover, char *buffer)
{
	char	*temp;

	if (*leftover)
	{
		temp = ft_strjoin(*leftover, buffer);
		free(*leftover);
		*leftover = temp;
	}
	else
		*leftover = ft_strdup(buffer);
	return (*leftover);
}

static char	*read_and_process(int fd, char **leftover)
{
	char	*buffer;
	ssize_t	bytes_read;
	int		newline_index;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		if (!update_leftover(leftover, buffer))
			return (free(buffer), NULL);
		newline_index = check_newline(*leftover);
		if (newline_index >= 0)
			return (free(buffer), extract_line(leftover, newline_index));
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	if (bytes_read == -1)
	{
		free(*leftover);
		*leftover = NULL;
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*leftover;
	char		*line;
	int			newline_index;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (leftover)
	{
		newline_index = check_newline(leftover);
		if (newline_index >= 0)
			return (extract_line(&leftover, newline_index));
	}
	line = read_and_process(fd, &leftover);
	if (line)
		return (line);
	if (leftover && *leftover)
	{
		line = ft_strdup(leftover);
		free(leftover);
		leftover = NULL;
		return (line);
	}
	free(leftover);
	leftover = NULL;
	return (NULL);
}
