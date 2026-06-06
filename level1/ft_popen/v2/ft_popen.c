/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 20:29:35 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/06 17:13:06 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdarg.h>

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

int		ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

void	ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

int close_all(int *f1, ...)
{
	va_list	args;
	int		*arg;

	va_start(args, f1);
	close(*f1);
	while ((arg = va_arg(args, int *)))
		close(*arg);
	va_end(args);
	return (1);
}

int	ft_popen(const char *file, char *const argv[], char type)
{
	int		ipp[2];
	int		opp[2];
	pid_t	pid;

	if ((type != 'r' && type != 'w')
		|| file == NULL
		|| argv == NULL || *argv == NULL
		|| pipe(ipp) == -1
		|| (pipe(opp) == -1 && close_all(ipp, ipp + 1, 0)))
		return (-1);
	pid = fork();
	if (pid == -1 && close_all(ipp, ipp + 1, opp, opp + 1, 0))
		return (-1);
	if (pid == 0)
	{
		if (close_all(ipp + 1, opp, 0) != 1
			|| (type == 'w' && dup2(ipp[0], 0) == -1)
			|| (type == 'r' && dup2(opp[1], 1) == -1))
		{
			close_all(ipp, opp + 1, 0);
			exit(1);
		}
		if (close_all(ipp, opp + 1, 0) && execvp(file, argv) == -1)
			exit(1);
	}
	if (type == 'r' && close_all(ipp, ipp + 1, opp + 1, 0))
		return (opp[0]);
	if (type == 'w' && close_all(ipp, opp, opp + 1, 0))
		return (ipp[1]);
	return (close_all(ipp, ipp + 1, opp, opp + 1, 0), -1);
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

// r -> returns a fd that reads the OUTPUT of the command
// w -> returns a fd that writes the INPUT of the command
int	main(void)
{
	char	buffer[BUFFER_SIZE];
	int		bytes;
	int		fd;
	char	**strs;

	for (int i = 0; i < BUFFER_SIZE; i++)
		buffer[i] = 0;
	fd = ft_popen("ls", (char *const []){"ls", "-l", NULL}, 'r');
	if (fd == -1)
		return (1);
	while ((bytes = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		printf("%s", buffer);
		for (int i = 0; i < BUFFER_SIZE; i++)
			buffer[i] = 0;
	}
	close(fd);
	strs = (char *[]){"Bonjour\n", "Hello\n", "Hej\n", "As-salamu alaykum\n",
		"Konnichiwa\n", "Aba\n", NULL};
	fd = ft_popen("nl", (char *const []){"nl", NULL}, 'w');
	if (fd == -1)
		return (1);
	for (int i = 0; strs[i]; i++)
		dprintf(fd, "%s", strs[i]);
	close(fd);
	return (0);
}
