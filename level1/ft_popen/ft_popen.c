/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 20:29:35 by jegerman          #+#    #+#             */
/*   Updated: 2026/03/25 01:51:44 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>

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
		if (bpos == 0 || bpos >= bytes)
		{
			if ((bytes = read(fd, buffer, BUFFER_SIZE)) == -1)
				return (free(line), NULL);
			if ((bpos > 0 && (bpos = 0)) || bytes == 0)
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

int	ft_popen(const char *file, char *const argv[], char type)
{
	int		ipp[2];
	int		opp[2];
	pid_t	pid;

	if (file == NULL
		|| argv == NULL || *argv == NULL
		|| (type != 'r' && type != 'w'))
		return (-1);

	if (pipe(ipp) == -1)
		return (-1);
	if (pipe(opp) == -1)
		return (close(ipp[0]), close(ipp[1]), -1);

	pid = fork();
	if (pid == -1)
		return (close(ipp[0]), close(ipp[1]), close(opp[0]), close(opp[1]), -1);
	if (pid == 0)
	{
		close(ipp[1]);
		close(opp[0]);

		// OK I see... dup2(ipp[0], 0) == -1 || 
		if (dup2(opp[1], 1) == -1)
			(void)(close(ipp[0]), close(opp[0]), exit(1));
		close(ipp[0]);
		close(opp[1]);
		if (execvp(argv[0], argv) == -1)
			exit(1);
		exit(0);
	}

	if (type == 'r')
	{
		(void)(close(ipp[0]), close(ipp[1]), close(opp[1]));
		return (opp[0]);
	}
	else if (type == 'w')
	{
		(void)(close(ipp[0]), close(opp[0]), close(opp[1]));
		return (ipp[1]);
	}

	return (close(ipp[0]), close(ipp[1]), close(opp[0]), close(opp[1]), -1);
}

// int main(void)
// {
//     int  fd;
//     char *line;

//     fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
//     while ((line = get_next_line(fd)))
// 	{
//         ft_putstr(line);
// 		free(line);
// 	}
//     return (0);
// }

int	main(void)
{
	char	*line;
	int		fd;
	
	fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
	dup2(fd, 0); // 25/03: It's him!
	fd = ft_popen("grep", (char *const []){"grep", "c", NULL}, 'r');
	
	while ((line = get_next_line(fd)))
	{
		printf("%s", line);
		free(line);
	}
	return (0);
}
