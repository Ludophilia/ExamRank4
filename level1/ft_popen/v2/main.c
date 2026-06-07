/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 18:50:56 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/07 21:53:05 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 10

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE > 0 ? BUFFER_SIZE : 1];
	static int	bpos, bytes;
	char		*line;
	int			lpos;

	if (fd < 0 || BUFFER_SIZE < 1 || !(line = malloc(1024 * 1024)))
		return (NULL);
	lpos = 0;
	while (1)
	{
		if (bpos == 0 || (bpos >= bytes && !(bpos = 0)))
		{
			if ((bytes = read(fd, buffer, BUFFER_SIZE)) == -1)
				return (free(line), NULL);
			if (bytes == 0)
				break ;
		}
		while (bpos < bytes)
		{
			if ((line[lpos++] = buffer[bpos++]) == '\n')
				return (line[lpos] = 0, line);
		}
	}
	return (lpos > 0 ? (line[lpos] = 0, line) : (free(line), NULL));
}

// r -> returns a fd that reads the OUTPUT of the command
// w -> returns a fd that writes the INPUT of the command
int	main(void)
{
	int		fd;
	int		ft_popen(const char *, char *const[], char);

	char		buffer[BUFFER_SIZE];
	int		bytes;

	for (int i = 0; i < BUFFER_SIZE; i++)
		buffer[i] = 0;
	fd = ft_popen("ls", (char *const []){"ls", "-l", NULL}, 'r');
	if (fd == -1)
		return (1);
	while ((bytes = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		printf("%s", buffer);
		for (int i = 0; i < bytes; i++)
			buffer[i] = 0;
	}
	close(fd);
	// exit (0);

	// char	**strs;

	// fd = ft_popen("nl", (char *const []){"nl", NULL}, 'w');
	// if (fd == -1)
	// 	return (1);
	// printf("[%i] ft_popen returned: mode -> '%c' ; fd -> %i\n", getpid(), 'w', fd);
	// strs = (char *[]){"Bonjour\n", "Hello\n", "Hej\n", "As-salamu alaykum\n",
	// 	"Konnichiwa\n", "Aba\n", NULL};
	// for (int i = 0; strs[i]; i++)
	// 	dprintf(fd, "%s", strs[i]);
	// close(fd);


	// fd = ft_popen("cat", (char *const []){"cat", NULL}, 'r');
	// if (fd == -1)
	// 	return (1);
	// printf("[%i] ft_popen returned: mode -> '%c' ; fd -> %i\n", getpid(), 'w', fd);
	// // strs = (char *[]){"Bonjour\n", "Hello\n", "Hej\n", "As-salamu alaykum\n",
	// // 	"Konnichiwa\n", "Aba\n", NULL};

	// char	*line = get_next_line(fd);
	// printf("line: %s", line);

	// // printf("Received: %s\n", buf);
	// close(fd);
	return (0);
}

// int	main(void)
// {
// 	char	*line;
// 	int		fd;
	
// 	fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
// 	dup2(fd, 0);
// 	fd = ft_popen("grep", (char *const []){"grep", "c", NULL}, 'r');
	
// 	while ((line = get_next_line(fd)))
// 	{
// 		printf("%s", line);
// 		free(line);
// 	}
// 	return (0);
// }