/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 16:00:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/07/03 22:18:09 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>

// 3/07: Error handling is incomplete...

// 3/07: Builtin cd management is still not yet functional...

int	builtin_cd(char *path)
{
	// changes directory
	chdir(path);
}

int	create_cmd(char **toks, int *cmds_total, char ***cmd)
{
	int		size;
	int		j;

	size = 0;
	while (toks[size]
		&& strncmp(toks[size], "|", 2) != 0
		&& strncmp(toks[size], ";", 2) != 0)
		size++;
	*cmd = malloc((size + 1) * sizeof(char *));
	if (*cmd == NULL)
		return (-1);
	j = -1;
	while (++j < size)
		(*cmd)[j] = toks[j];
	(*cmd)[j] = 0;
	return (*cmds_total += 1, size);
}

int exec_cmd(int *pi, char **cmd, int cmds_nb, char **envp)
{
	// cur_arg, cmds_nb...

	// Reasons:

	// cur_arg -> helps with understand if there's something after the pipe.
	//		= And what if there NO pipe?

	//     There's a problem here.


	// cmds_nb -> helps understanding the command's position in the 
	// pipeline and the necessity to use last pipe's read end at pi[2]
	
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
		free(cmd);
		exit(1);

	}
	if (cur_arg && strncmp(cur_arg, ";", 2) != 0)
		close(pi[1]);
	if (cmds_nb > 1)
		close(pi[2]);

	return (0);
}

// return the offset or -1. I want to see i making progress.
// int	exec_pipeline(int *i, int argc, char **argv, char **envp)
int	exec_pipeline(char **toks, char **envp)
{
	// BLOATED
	int		cmds_total;
	int		i;
	int		toks_usd;
	int		pi[3];
	char	**cmd;
	
	int		wstatus;

	i = 0;
	cmds_total = 0;
	while (toks[i] && strncmp(toks[i], ";", 2) != 0)
	{
		if ((toks_usd = create_cmd(toks + i, &cmds_total, &cmd)) == -1)
			return (-1);
		i += toks_usd;
		
		if (toks[i] && strncmp(toks[i], "|", 2) == 0)
		{
			pipe(pi); // FEAR ME. I WILL HAUNT YOUR DEBUGGING EFFORTS.
			i++;
		}

		// What's the problem here?

		// We have to exec the command.
		// 
	
		exec_cmd(pi, toks[i], cmd, cmds_total, envp);

		free(cmd);
		pi[2] = toks[i] && strncmp(toks[i] , ";", 2) != 0 ? pi[0] : -1;
	}
	
	// wstatus error handling...
	for (int j = 0; j < cmds_total; j++)
		waitpid(-1, &wstatus, 0);
	return (0);
}


// $>./microshell /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell
// microshell
// i love my microshell

// ./microshell /usr/bin/sleep 10 "|" /usr/bin/echo a ";" /usr/bin/echo b

// ./microshell /usr/bin/echo a ";" /usr/bin/echo b
// ./microshell /usr/bin/echo a ";"

int microshell(char **toks, char **envp)
{
	int		i;
	int		toks_usd;

	i = 0;
	while (toks[i])
	{
		if ((toks_usd = exec_pipeline(toks + i, envp)) == -1)
			return (-1);
		i += toks_usd;
		if (toks[i] && strncmp(toks[i], ";", 2) == 0)
			i++;
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	if (argc == 1
		|| microshell(++argv, envp) == -1)
		return (1);
	return (0);
}
