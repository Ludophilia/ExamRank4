/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 01:11:36 by jegerman          #+#    #+#             */
/*   Updated: 2026/03/28 00:52:21 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

// int main(void)
// {
//     int fds[2];
    
//     for (int i = 0; i < 3 ; i++)
//     {
//         pipe(fds);
//         printf("fds[0] -> %i; fds[1] -> %i\n", fds[0], fds[1]);
//         close(fds[0]); close(fds[1]);
//     }

//     return (0);
// }

int	picoshell(char **cmds[])
{
	int		wstatus, pi[2], prev_rpi;
	pid_t	pid;

	// NEXT:
	// - Function argument error management
	// - fd management
	if (cmds == NULL)
		return (1);

	*(long *)pi = 0;
	for (int i = 0; cmds[i]; i++)
	{
		// {cmds[0] <4---pipe---[3]>} {cmds[1] <6---pipe---[5]>} {cmds[2]
		prev_rpi = (i == 0 ? 0 : pi[0]);
		if (cmds[i + 1] != NULL && pipe(pi) == -1)
			return (1);
		// printf("==%i== Refreshed: pi[0] -> %i; pi[1] -> %i; prev_rpi -> %i\n",
		// 	getpid(), pi[0], pi[1], prev_rpi);
		pid = fork();
		if (pid == -1 && printf("e1\n"))
			return (1);
		if (pid == 0)
		{
			// (void)prev_rpi;
			
			if (pi[0] != 0 && pi[0] != prev_rpi)
			{
				// printf("pi[0] -> %i\n", pi[0]);
				close(pi[0]); //
			}
			if (prev_rpi != 0)
			{
				// printf("prev_rpi -> %i\n", prev_rpi);
				if (dup2(prev_rpi, 0) == -1 && printf("e2\n"))
					exit(1);
				close(prev_rpi); //
			}
			if (cmds[i + 1] != NULL)
			{
				if (dup2(pi[1], 1) == -1 && printf("e3\n"))
					exit(1);
				close(pi[1]); //
			}
			if (execvp(cmds[i][0], cmds[i]) == -1 && printf("e0\n"))
				exit(1);
			exit(0);
		}
		if (pid != 0)
		{
			if (pi[1] != 0)
				close(pi[1]);
			if (prev_rpi != 0)
				close(prev_rpi);
			if (wait(&wstatus) == -1
				|| WEXITSTATUS(wstatus) == 1)
				return (printf("e4\n"), 1);
		}
	}
	return (0);
}

// ./picoshell /bin/ls "|" /usr/bin/grep picoshell
// picoshell
// ./picoshell echo 'squalala' "|" cat "|" sed 's/a/b/g'
// squblblb
int	main(void)
{
	// The argv array will be transformed to a char	*** like those below
	// following pipes position.
	char	***cmds1, ***cmds2;

	cmds1 = (char **[])
	{
		(char *[]){"echo", "squalala", NULL},
		(char *[]){"nl", NULL},
		(char *[]){"cat", NULL},
		(char *[]){"tac", NULL},
		(char *[]){"rev", NULL},
		(char *[]){"tee", NULL},
		(char *[]){"nl", NULL},
		(char *[]){"rev", NULL},
		(char *[]){"sed", "s/a/b/g", NULL},
		(char *[]){"rev", NULL},
		NULL
	};
	cmds2 = (char **[])
	{
		(char *[]){"/bin/ls", NULL},
		(char *[]){"/usr/bin/grep", "picoshell", NULL},
		NULL
	};
	if (picoshell(cmds1) == 1)
		return (1);

	(void)cmds1;
	(void)cmds2;

	return (0);
}
