/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 16:00:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/07/04 23:09:40 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>

typedef struct s_cnt
{
	int		cmds;
	int		pi;
}	t_cnt;

// 3/07: Error handling is incomplete...

// 3/07: Builtin cd management is still not yet functional...

int	builtin_cd(char *path)
{
	// changes directory
	chdir(path);
}

char **create_cmd(char **toks, t_cnt *ct, int *i)
{
	char	**cmd;
	int		size;
	int		j;

	size = 0;
	while (toks[size]
		&& strncmp(toks[size], "|", 2) != 0
		&& strncmp(toks[size], ";", 2) != 0)
		size++;
	cmd = malloc((size + 1) * sizeof(char *));
	if (cmd == NULL)
		return (-1);
	j = -1;
	while (++j < size)
		cmd[j] = toks[j];
	cmd[j] = 0;
	return (ct->cmds += 1, *i += size, cmd);
}

int exec_cmd(char **cmd, t_cnt *ct, int *pi, char **envp)
{	
	pid_t	pid;

	if ((pid = fork()) == -1)
		return (-1);
	if (pid == 0)
	{

		// if there's a new pipe just opened before? (That's it?)
		if (ct->pi > 0)
			close(pi[0]);

		// int cmds_nb -> helps understanding the command's position in the 
		// pipeline and the necessity to use last pipe's read end at pi[2]
		// if there's at least two commands, meaning there are two pipes opened,
		// the first one likely only with its read end...
		if (ct->cmds > 1)
		{
			dup2(pi[2], 0);
			close(pi[2]);
		}
		if (ct->pi > 0)
		{
			dup2(pi[1], 1);
			close(pi[1]);
		}
	
		execve(*cmd, cmd, envp);
		free(cmd);
		exit(1);

	}


	if (ct->pi > 0)
		close(pi[1]);
	if (ct->cmds > 1)
		close(pi[2]);

	return (0);
}

int	exec_pipeline(char **toks, char **envp)
{
	// (LESS) BLOATED
	char	**cmd;
	int		pi[3];
	int		wstatus;
	t_cnt	ct;
	int		i;
	
	i = 0;
	ct.cmds = 0;
	ct.pi = 0;
	while (toks[i] && strncmp(toks[i], ";", 2) != 0)
	{
		// Current: Pipeline with 1 command, no pipe. What's supposed to happen.
	
		if ((cmd = create_cmd(toks + i, &ct, &i)) == NULL)
			return (-1);


		if (toks[i] && strncmp(toks[i], "|", 2) == 0)
		{
			pipe(pi);
			i++;
			ct.pi++;
		}
		
		exec_cmd(cmd, &ct, pi, envp);
	

		free(cmd);		
		pi[2] = ct.pi > 0 ? ct.pi--, pi[0] : -1;
	}
	
	// wstatus error handling...
	for (int j = 0; j < ct.cmds; j++)
		waitpid(-1, &wstatus, 0);

	// Reminder: We're supposed to return the number of token consumed...
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
