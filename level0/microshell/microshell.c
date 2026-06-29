/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 16:00:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/29 21:47:48 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <wait.h>

// Builtin c

// ==

char **create_cmd(char **argv, int *i)
{
	int		size;
	char	**cmd;
	int		j;

	argv += *i;
	size = 0;
	while (argv[size]
		&& strncmp(argv[size], "|", 2) != 0
		&& strncmp(argv[size], ";", 2) != 0)
		size++;
	cmd = malloc((size + 1) * sizeof(char *));
	if (cmd == NULL)
		return (NULL);
	j = -1;
	while (++j < size)
		cmd[j] = argv[j]; // No strdup?
	cmd[j] = 0;
	return (*i += size, cmd);
}

// $>./microshell /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell
int microshell(int argc, char **argv, char **envp)
{
	int 	i;
	
	int 	wstatus;
	pid_t 	pid;
	
	int 	fds[2];
	char 	**cmds;

	i = 0;
	while (i < argc)
	{
		// printf("%s\n", argv[i++]);

		if ((cmds = create_cmd(argv, &i)) == NULL)
			return (-1);

		if (strncmp(argv[i], "|", 2) == 0)
			pipe(fds); // i++; ???

		// while ()

		if ((pid = fork()) == -1)
			return (-1);

		if (pid == 0)
		{
			close();
			dup2( , 0);
			close();
			dup2( , 1);

			// if (command == cd) // there's a pipe...?
			//	....
			// else
			// execve(*cmds, cmds, envp); // char **cmds
			// exit(1);
		}

		// waitpid(pid, &wstatus, 0);

		
		free(cmds); // free the char **?

		i++; // WTF?
	}

	return (0);
}

int main(int argc, char **argv, char **envp)
{
	if (argc == 1)
		return (1);

	microshell(--argc, ++argv, envp);

	// return ();
}

// $>./microshell /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell
// microshell
// i love my microshell