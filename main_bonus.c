/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtomanar <mtomanar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 12:00:00 by mtomanar          #+#    #+#             */
/*   Updated: 2026/09/08 12:00:00 by mtomanar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>

static void	print_line(const char *prefix, char *line)
{
	if (line)
		printf("%s%s", prefix, line);
	else
		printf("%s(NULL)\n", prefix);
}

static int	open_three(int fds[3])
{
	fds[0] = open("lorem.txt", O_RDONLY);
	fds[1] = open("lorem.txt", O_RDONLY);
	fds[2] = open("get_next_line_bonus.h", O_RDONLY);
	if (fds[0] == -1 || fds[1] == -1 || fds[2] == -1)
		return (0);
	return (1);
}

static void	handle_fd(int fd, int idx, int *done)
{
	char	*line;

	line = get_next_line(fd);
	if (line)
	{
		if (idx == 0)
			print_line("[fd1] ", line);
		else if (idx == 1)
			print_line("[fd2] ", line);
		else
			print_line("[fd3] ", line);
		free(line);
	}
	else
	{
		printf("[fd%d] -> EOF\n", idx + 1);
		*done = 1;
	}
}

static void	interleaved(int fds[3])
{
	int	done[3];
	int	i;

	done[0] = 0;
	done[1] = 0;
	done[2] = 0;
	while (!done[0] || !done[1] || !done[2])
	{
		i = 0;
		while (i < 3)
		{
			if (!done[i])
				handle_fd(fds[i], i, &done[i]);
			i++;
		}
		printf("--- round ---\n");
	}
}

int	main(void)
{
	int		fds[3];
	char	*line;

	if (!open_three(fds))
	{
		printf("Error opening files\n");
		return (1);
	}
	printf("=== BONUS: Multiple FD test ===\n");
	interleaved(fds);
	printf("\n=== Sequential test ===\n");
	close(fds[0]);
	fds[0] = open("lorem.txt", O_RDONLY);
	line = get_next_line(fds[0]);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fds[0]);
	}
	printf("\n[done]\n");
	close(fds[0]);
	close(fds[1]);
	close(fds[2]);
	return (0);
}
