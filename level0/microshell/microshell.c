/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 16:00:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/07/02 22:04:41 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>

// 30/06: Error handling is missing...


// 30/06: Builtin cd management is missing...

int	builtin_cd(char *path)
{
	// changes directory
	chdir(path);
}


// 30/06: ';' management is missing for now...

// WHat's for?

// Separates pipelines.
//	sleep 10 | sleep 11 ; sleep 12
//		sleep 10 and 11 are executed first in parallel, THEN sleep 12 alone
// sleep 10 | echo a ; echo b
//		sleep 10 and echo a are executed first in parallel. a is printed on
// 		stdout. b is printed once the pipeline is done.

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

int exec_cmd(int *pi, char *cur_arg, char **cmd, int cmds_nb, char **envp) // Norm!! (5 args)
{
	pid_t	pid;

	if ((pid = fork()) == -1)
		return (-1);
	if (pid == 0)
	{
		if (cur_arg && strncmp(cur_arg, ";", 2) != 0)
			close(pi[0]);
		if (cmds_nb > 1)
		{
			dup2(pi[2], 0);
			close(pi[2]);
		}
		if (cur_arg && strncmp(cur_arg, ";", 2) != 0)
		{
			dup2(pi[1], 1);
			close(pi[1]);
		}
		execve(*cmd, cmd, envp);
		exit(1);

	}
	if (cur_arg && strncmp(cur_arg, ";", 2) != 0)
		close(pi[1]);
	if (cmds_nb > 1)
		close(pi[2]);

	return (0);
}

int	exec_pipeline(int *i, int argc, char **argv, char **envp)
{
	int		pi[3];
	int		cmds_nb;
	char	**cmd;
	int		wstatus;

	cmds_nb = 0;
	while (*i < argc && strncmp(argv[*i], ";", 2) != 0)
	{
		
		if ((cmd = create_cmd(argv + *i, i, &cmds_nb)) == NULL)
			return (-1);

		if (argv[*i] && strncmp(argv[*i], "|", 2) == 0)
			(void)(pipe(pi), *i += 1);
	
		exec_cmd(pi, argv[*i], cmd, cmds_nb, envp);

		free(cmd);
		pi[2] = argv[*i]  && strncmp(argv[*i] , ";", 2) != 0 ? pi[0] : -1;
	}
	for (int j = 0; j < cmds_nb; j++)
		waitpid(-1, &wstatus, 0);
	return (0);
}

// $>./microshell /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell
// microshell
// i love my microshell


// ./microshell /usr/bin/sleep 10 "|" /usr/bin/echo a ";" /usr/bin/echo b

// ./microshell /usr/bin/echo a ";" /usr/bin/echo b
// ./microshell /usr/bin/echo a ";"

int microshell(int argc, char **argv, char **envp)
{	
	int 	i;

	i = 0;

	while (i < argc)
	{
		exec_pipeline(&i, argc, argv, envp);
		if (argv[i] && strncmp(argv[i], ";", 2) == 0)
			i++;
	}

	return (0);
}

int main(int argc, char **argv, char **envp)
{
	if (argc == 1)
		return (1);

	microshell(--argc, ++argv, envp);

	return (0);
}
