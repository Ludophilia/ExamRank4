/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 16:00:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/07/08 22:27:56 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>
// #include <stdbool.h>

typedef struct s_cnt
{
	int		cmds;
	int		pi;
	int		toks;
}	t_cnt;

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	ft_pipe(int *fds, int *i, t_cnt *ct)
{
	*i += 1;
	ct->pi = 1;
	return (pipe(fds));
}

int	builtin_cd(char **cmd, int size, t_cnt *ct)
{
	ct->cmds =- 1;
	if (size != 2)
	{
		write(2, "error: cd: bad arguments\n", 25);
		return (-1);
	}
	if (chdir(cmd[1]) == -1)
	{
		write(2, "error: cd: cannot change directory to ", 38);
		write(2, cmd[1], ft_strlen(cmd[1]));
		write(2, "\n", 1);
		return (-1);
	}
	return (0);
}

int	create_cmd(char **toks, char ***cmd, t_cnt *ct)
{
	int		size;
	int		j;

	size = 0;
	while (toks[size]
		&& strncmp(toks[size], "|", 2)
		&& strncmp(toks[size], ";", 2))
		size++;
	*cmd = malloc((size + 1) * sizeof(char *));
	if (*cmd == NULL)
		return (-1);
	ct->cmds++;
	j = -1;
	while (++j < size)
		(*cmd)[j] = toks[j];
	(*cmd)[j] = 0;
	return (ct->toks = size);
}

int	fatal_err(void)
{
	write(2, "error: fatal\n", 14);
	exit(1);
}

// 3/07: Error handling is incomplete...

int exec_cmd(char **cmd, t_cnt *ct, int *pi, char **envp)
{	
	pid_t	pid;

	

	
	if ((pid = fork()) == -1)
		return (-1);
	if (pid == 0)
	{
		if (ct->pi > 0 && close(pi[0]) == -1)
			return (-1); // exit

		if (ct->cmds > 1 && dup2(pi[2], 0) == -1)
			return (-1);
		if (ct->cmds > 1 && close(pi[2]) == -1)
			return (-1);

		// if (ct->cmds > 1)
		// {
		// 	dup2(pi[2], 0);
		// 	close(pi[2]);
		// }

		if (ct->pi > 0 && dup2(pi[1], 1) == -1)
			return (-1);
		if (ct->pi > 0 && close(pi[1]) == -1)
			return (-1);

		// if (ct->pi > 0)
		// {
		// 	dup2(pi[1], 1);
		// 	close(pi[1]);
		// }

		
		// 8/07: Error handling, again... And again.
		execve(*cmd, cmd, envp);
		write(2, "error: cannot execute ", 22);
		write(2, *cmd, ft_strlen(*cmd));
		free(cmd);
		exit(1);
	}

	if (ct->pi > 0 && close(pi[1]) == -1)
		return (-1);
	if (ct->cmds > 1 && close(pi[2]) == -1)
		return (-1);

	return (0);
}

// #include <errno.h> // What?

int	wait_pipeline(t_cnt *ct)
{
	int	errors;
	int	wstatus;

	errors = 0;
	for (int j = 0; j < ct->cmds; j++)
		if (waitpid(-1, &wstatus, 0) == -1
			|| WEXITSTATUS(wstatus) == 1)
			errors++;
	if (errors > 0)
		return (-1);
	return (0);
}

// 3/07, 8/07: Error handling is still incomplete... 
// Where are the error messages?
int	exec_pipeline(char **toks, char **envp, int *toks_usd)
{
	char	**cmd;
	int		pi[3];
	t_cnt	ct;
	int		i;
	int		is_blt;

	i = 0;
	ct.cmds = 0;
	while (toks[i] && strncmp(toks[i], ";", 2))
	{
		if (create_cmd(toks + i, &cmd, &ct) == -1)
			return (-1);
		i += ct.toks;
		if (toks[i] && !strncmp(toks[i], "|", 2)
			&& ft_pipe(pi, &i, &ct) == -1)
			return (-1); // ct.cmds > 1 && close(pi[2]), 
		if ((is_blt = !strncmp(*cmd, "cd", 3))
			&& builtin_cd(cmd, ct.toks, &ct) == -1)
			return (-1);
		if (is_blt == 0
			&& exec_cmd(cmd, &ct, pi, envp) == -1)
			return (-1);
		free(cmd);
		pi[2] = ct.pi > 0 ? (ct.pi--, pi[0]) : -1;
	}
	if (wait_pipeline(&ct) == -1)
		return (-1);
	return (*toks_usd = i);
}

int microshell(char **toks, char **envp)
{
	int		i;
	t_cnt	ct;

	i = 0;
	while (toks[i])
	{
		if (exec_pipeline(toks + i, envp, &ct.toks) == -1)
			return (-1);
		i += ct.toks;
		if (toks[i] && !strncmp(toks[i], ";", 2))
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

// $>./microshell /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell
// microshell
// i love my microshell

// ./microshell /usr/bin/sleep 10 "|" /usr/bin/echo a ";" /usr/bin/echo b

// ./microshell /usr/bin/echo a ";" /usr/bin/echo b
// ./microshell /usr/bin/echo a ";"

/* 
valgrind --track-fds=yes ./microshell /usr/bin/last 
"|" /usr/bin/head -20 "|" /usr/bin/head "|" /usr/bin/nl "|" /usr/bin/tac ";"
*/

// valgrind --track-fds=yes ./microshell /usr/bin/pwd ";" cd .. ";" /usr/bin/pwd

/*
valgrind --track-fds=yes ./microshell /usr/bin/pwd ";" cd .. ";" /usr/bin/pwd
";" cd .. ";" /usr/bin/pwd ";" cd .. ";" /usr/bin/pwd 
*/


// int exec_cmd(char **cmd, t_cnt *ct, int *pi, char **envp)
// {	
// 	pid_t	pid;

// 	if ((pid = fork()) == -1)
// 		return (-1);
// 	if (pid == 0)
// 	{
// 		if (ct->pi > 0)
// 			close(pi[0]);

// 		if (ct->cmds > 1)
// 		{
// 			dup2(pi[2], 0);
// 			close(pi[2]);
// 		}
// 		if (ct->pi > 0)
// 		{
// 			dup2(pi[1], 1);
// 			close(pi[1]);
// 		}

// 		execve(*cmd, cmd, envp);
// 		free(cmd);
// 		exit(1);
// 	}

// 	if (ct->pi > 0)
// 		close(pi[1]);
// 	if (ct->cmds > 1)
// 		close(pi[2]);

// 	return (0);
// }

// int	exec_pipeline(char **toks, char **envp, int *toks_usd)
// {
// 	char	**cmd;
// 	int		pi[3];
// 	int		wstatus;
// 	t_cnt	ct;
// 	int		i;
	
// 	i = 0;
// 	ct.cmds = 0;
// 	ct.pi = 0;
// 	while (toks[i] && strncmp(toks[i], ";", 2) != 0)
// 	{
// 		if (create_cmd(toks + i, &cmd, &ct) == -1)
// 			return (-1);
// 		i += ct.toks;
// 		if (toks[i] && strncmp(toks[i], "|", 2) == 0)
// 		{
// 			// error handling: use picoshell's solution
// 			if (pipe(pi) == -1) 
// 				return (ct.cmds > 1 && close(pi[2]), -1);
// 			ct.pi++;
// 			i++;
// 		}
// 		if (strncmp(*cmd, "cd", 3) == 0)
// 			builtin_cd(cmd, ct.toks);
// 		else
// 			exec_cmd(cmd, &ct, pi, envp);
// 		free(cmd);
// 		pi[2] = ct.pi > 0 ? (ct.pi--, pi[0]) : -1;
// 	}
	
// 	for (int j = 0; j < ct.cmds; j++)
// 		waitpid(-1, &wstatus, 0) == -1;
// 	return (*toks_usd = i);
// }
