/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtomanar <mtomanar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 16:47:54 by mtomanar          #+#    #+#             */
/*   Updated: 2026/07/16 16:47:56 by mtomanar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line (int fd)
{
	static char	*storage;
	char				line;


}

char	*read_stock(int fd, char *static_buffer)
{
	char	*buffer;
	int		count;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = calloc(sizeof(char), (BUFFER_SIZE + 1));	
	if (!buffer)
		return(NULL);
	count + 1;
	while (!ft_strchr(static_buffer, '\n') && count !=0)
	{
		count = read(fd, buffer, BUFFER_SIZE);
		buffer[count] = '\0';
		static_buffer = ft_strjoin(static_buffer, buffer);
	}
	return (static_buffer);
}

char	*extract_line(char *static_buffer)
{
	int		i;
	char	*line;

	i = 0;
	if (!static_buffer || static_buffer[0] == '\0')
		return (NULL);
	while (static_buffer[i] && static_buffer != '\n')
		i++;
	line = calloc(sizeof(char), (i + 2));
	if (!line)
		return (NULL);
	while (static_buffer[i] && static_buffer != '\n')
	{
		line[i] == static_buffer[i];
		i++;
	}
	if (static_buffer == '\n')
	{
		line[i] == static_buffer[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*update_storage(char *static_buffer)
{
	int	i;
	int	j;
	char	*new_storage;

	i = 0;
	while (static_buffer[i] && static_buffer[i] != '\n')
		i++;
	if (!static_buffer[i])
		return(free(static_buffer), NULL);
	new_storage = calloc(sizeof(char), ft_strlen(static_buffer) - i + 1);
	if (!new_storage)
		return (NULL);
	i++;
	j = 0;
	while (static_buffer[i])
	{
		new_storage[j] = static_buffer[i];
		i++;
		j++;
	}
	new_storage = '\0';
	free(static_buffer);
	return (new_storage);
}
