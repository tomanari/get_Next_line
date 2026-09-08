/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtomanar <mtomanar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 16:47:54 by mtomanar          #+#    #+#             */
/*   Updated: 2026/09/08 12:00:00 by mtomanar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*read_stock(int fd, char *static_buffer)
{
	char	*buffer;
	char	*tmp;
	int		count;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	count = 1;
	while (!ft_strchr(static_buffer, '\n') && count != 0)
	{
		count = read(fd, buffer, BUFFER_SIZE);
		if (count == -1)
			return (free(buffer), free(static_buffer), NULL);
		buffer[count] = '\0';
		tmp = ft_strjoin(static_buffer, buffer);
		if (!tmp)
			return (free(buffer), NULL);
		static_buffer = tmp;
	}
	return (free(buffer), static_buffer);
}

char	*extract_line(char *static_buffer)
{
	int		i;
	int		len;
	char	*line;

	i = 0;
	if (!static_buffer || !static_buffer[0])
		return (NULL);
	while (static_buffer[i] && static_buffer[i] != '\n')
		i++;
	len = i + (static_buffer[i] == '\n');
	line = malloc(len + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (static_buffer[i] && static_buffer[i] != '\n')
	{
		line[i] = static_buffer[i];
		i++;
	}
	if (static_buffer[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*update_storage(char *static_buffer)
{
	int		i;
	int		j;
	char	*new_storage;

	i = 0;
	if (!static_buffer)
		return (NULL);
	while (static_buffer[i] && static_buffer[i] != '\n')
		i++;
	if (!static_buffer[i])
		return (free(static_buffer), NULL);
	new_storage = malloc(ft_strlen(static_buffer) - i);
	if (!new_storage)
		return (free(static_buffer), NULL);
	i++;
	j = 0;
	while (static_buffer[i])
		new_storage[j++] = static_buffer[i++];
	new_storage[j] = '\0';
	return (free(static_buffer), new_storage);
}

char	*get_next_line(int fd)
{
	static char	*storage[FD_MAX];
	char		*line;

	if (fd < 0 || fd >= FD_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	storage[fd] = read_stock(fd, storage[fd]);
	if (!storage[fd])
		return (NULL);
	line = extract_line(storage[fd]);
	if (!line)
	{
		free(storage[fd]);
		storage[fd] = NULL;
		return (NULL);
	}
	storage[fd] = update_storage(storage[fd]);
	return (line);
}
