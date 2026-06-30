/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 16:00:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/30 21:00:04 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>

// 30/06: Builtin cd management is missing...

// 30/06: Error handling is missing...

char **create_cmd(char **argv, int *i, int *cmd_nb)
{
	int		size;
	char	**cmd;
	int		j;

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
		cmd[j] = argv[j];
	cmd[j] = 0;
	return (*i += size, *cmd_nb += 1, cmd);
}

// $>./microshell /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell
// microshell
// i love my microshell
int microshell(int argc, char **argv, char **envp)
{	
	int 	wstatus;
	pid_t 	pid;

	int 	pi[2];
	int		last_pi0;
	char 	**cmds;

	int 	i;
	int		cmds_nb;

	i = 0;
	cmds_nb = 0;

	// 30/06: ';' management is missing for now...
	while (i < argc)
	{
		// printf("[1] i -> %i / argc -> %i\n", i, argc);
		// printf("%s\n", argv[i]);
		// fflush(NULL);

		if ((cmds = create_cmd(argv + i, &i, &cmds_nb)) == NULL)
			return (-1);

		// printf("[2] i -> %i / argc -> %i\n", i, argc);
		// printf("%s\n", argv[i]);
		// fflush(NULL);
			
		if (argv[i] && strncmp(argv[i], "|", 2) == 0)
			(void)(pipe(pi), i++); // ???

		if ((pid = fork()) == -1)
			return (-1);

		if (pid == 0)
		{
			if (argv[i])
				close(pi[0]);

			if (cmds_nb > 1)
			{
				dup2(last_pi0, 0);
				close(last_pi0);
			}
			if (argv[i])
			{
				dup2(pi[1], 1);
				close(pi[1]);
			}

			execve(*cmds, cmds, envp); // char **cmds
			exit(1);
		}

		free(cmds);
		
		if (argv[i])
			close(pi[1]);
		if (cmds_nb > 1)
			close(last_pi0);
		

		last_pi0 = argv[i] ? pi[0] : -1;
	}

	for (int j = 0; j < cmds_nb; j++)
		waitpid(pid, &wstatus, 0);

	return (0);
}

int main(int argc, char **argv, char **envp)
{
	if (argc == 1)
		return (1);

	microshell(--argc, ++argv, envp);

	return (0);
}
