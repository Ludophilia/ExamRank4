/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 01:11:36 by jegerman          #+#    #+#             */
/*   Updated: 2026/03/27 01:47:12 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


#include <stdio.h>

int	picoshell(char **cmds[])
{
	// It's still a challenge by itself to find an efficient way to manage 
	// those pipes...
	int		pi[2], last_r;
	pid_t	pid;

	if (cmds == NULL) // more?
		return (1);

	// cmds[0]... cmds[1]... cmds[n].
	for (int i = 0; cmds[i]; i++)
	{
		//	cmds[0]: read in 0 (i == 0), write in pi[1],
		//	cmds[1]: read in last_r (pi[0]), write in u_pi[1],
		//  cmds[2]: read in last_r (pi[0]), write in 1,
		
		// (i + 1 != NULL)

		last_r = (i == 0 ? 0 : pi[0]);
	
		if (cmds[i + 1] != NULL && pipe(pi) == -1)
			return (1); // close fds?
		printf("==%i== Refreshed: pi[0] -> %i; pi[1] -> %i; last_r -> %i\n",
			getpid(), pi[0], pi[1], last_r);
		pid = fork();
		if (pid == -1)
			return (1); // close fds?
		if (pid == 0)
		{
			if (last_r != 0)
			{
				printf("==%i== [%i] last_r -> %i\n", getpid(), i, last_r);
				if (dup2(last_r, 0) == -1) // shit tier err management
					return (printf("dup2 fail last r\n"), exit(1), 1);
				close(last_r);
			}
			if (cmds[i + 1] != NULL)
			{
				printf("==%i== [%i] pi[1] -> %i, pi[0] -> %i\n", getpid(), i, pi[1], pi[0]);
				if (dup2(pi[1], 1) == -1) // shit tier err management
					return (printf("dup2 fail last r\n"), exit(1), 1);
				
				
					
				close(pi[1]);
				close(pi[0]);
			}
			// execvp(cmds[i][0], cmds[i]);
			// printf("i -> %i. IT DIDN'T WORK...\n", i);
			// exit(1);
			exit(0);
		}
		if (pid != 0)
		{
			int		wstatus;
			
			close(pi[1]);
			close(last_r);
			wait(&wstatus);
			if (WEXITSTATUS(wstatus) == 1)
				return (1); // close
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
	// The argv array will be transformed to a
	char	***cmds1, ***cmds2;

	cmds1 = (char **[])
	{
		(char *[]){"echo", "squalala", NULL},
		(char *[]){"cat", NULL},
		(char *[]){"nl", NULL},
		NULL,
		(char *[]){"sed", "'s/a/b/g'", NULL},
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

	(void)cmds2;

	return (0);
}
