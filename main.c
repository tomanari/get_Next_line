/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtomanar <mtomanar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 16:50:24 by mtomanar          #+#    #+#             */
/*   Updated: 2026/07/24 22:03:56 by mtomanar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	*line;

	/* Tente abrir um arquivo de texto qualquer que você tenha criado na pasta*/
	fd = open("lorem.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("Erro ao abrir o arquivo\n");
		return (1);
	}
	/* Loop para chamar o GNL várias vezes até ele retornar NULL*/
	line = get_next_line(fd);
	while (line)
	{
		line = get_next_line(fd);
		printf("%s", line); /* Lembrete: a 'line' já deve ter o \n!*/
		free(line); /*O usuário que chamou o GNL tem que dar free!*/
	}
	close(fd);
	return (0);
}
