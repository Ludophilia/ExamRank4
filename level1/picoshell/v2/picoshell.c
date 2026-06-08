/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 01:11:36 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/08 22:30:07 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int	smrt_close(int *fd)
{
	close(*fd);
	*fd = 0;
	return (1);
}

int close_all(int pi[2], int prev_rpi)
{
	if (pi[0] != 0 && pi[0] != prev_rpi)
		close(pi[0]);
	if (prev_rpi != 0)
		close(prev_rpi);
	if (pi[1] != 0)
		close(pi[1]);
	return (1);
}

// 8/06. I want to get rid of those close all and smart close functions...
int	picoshell(char **cmds[])
{
	int		wstatus, pi[2] = {0, 0}, prev_rpi = 0;
	pid_t	pid;

	if (cmds == NULL)
		return (1);

	// {cmds[0] <4---pipe} 
	// {---[3]>cmds[1]<6---pipe} 
	// {---[5]>cmds[2]...}

	for (int i = 0; cmds[i]; i++)
	{
		if (cmds[i] == NULL || cmds[i][0] == NULL)
			return (close_all(pi, prev_rpi), 1);

		prev_rpi = (i > 0 ? pi[0]: 0);
		
		if (cmds[i + 1] != NULL && pipe(pi) == -1 && close_all(pi, prev_rpi))
			return (1);

	
		pid = fork();
		if (pid == -1 && close_all(pi, prev_rpi))
			return (1);


		if (pid == 0)
		{
			if (pi[0] != 0 && pi[0] != prev_rpi)
				smrt_close(pi + 0);
			if (prev_rpi != 0)
			{
				if (dup2(prev_rpi, 0) == -1 && close_all(pi, prev_rpi))
					exit(1);
				smrt_close(&prev_rpi);
			}
			if (cmds[i + 1] != NULL)
			{
				if (dup2(pi[1], 1) == -1 && close_all(pi, prev_rpi))
					exit(1);
				smrt_close(pi + 1);
			}
			if (execvp(cmds[i][0], cmds[i]) == -1 && close_all(pi, prev_rpi))
				exit(1);
		}
	
		if (pid != 0)
		{
			if (pi[1] != 0)
				smrt_close(pi + 1);
			if (prev_rpi != 0)
				smrt_close(&prev_rpi);
			if (wait(&wstatus) == -1 || WEXITSTATUS(wstatus) == 1)
				return (close_all(pi, prev_rpi), 1);
		}
	}
	return (0);
}



int	smrt_close(int *fd)
{
	close(*fd);
	*fd = 0;
	return (1);
}

int close_all(int pi[2], int prev_rpi)
{
	if (pi[0] != 0 && pi[0] != prev_rpi)
		close(pi[0]);
	if (prev_rpi != 0)
		close(prev_rpi);
	if (pi[1] != 0)
		close(pi[1]);
	return (1);
}

// printf("==%i== Refreshed: pi[0] -> %i; pi[1] -> %i; prev_rpi -> %i\n",
// 	getpid(), pi[0], pi[1], prev_rpi);
int	picoshell(char **cmds[])
{
	int		wstatus, pi[2] = {0, 0}, prev_rpi = 0;
	pid_t	pid;

	if (cmds == NULL)
		return (1);

	// {cmds[0] <4---pipe} 
	// {---[3]>cmds[1]<6---pipe} 
	// {---[5]>cmds[2]...}

	for (int i = 0; cmds[i]; i++)
	{
		if (cmds[i] == NULL || cmds[i][0] == NULL)
			return (close_all(pi, prev_rpi), 1);

		prev_rpi = (i > 0 ? pi[0]: 0);
		
		if (cmds[i + 1] != NULL && pipe(pi) == -1 && close_all(pi, prev_rpi))
			return (1);

	
		pid = fork();
		if (pid == -1 && close_all(pi, prev_rpi))
			return (1);


		if (pid == 0)
		{
			if (pi[0] != 0 && pi[0] != prev_rpi)
				smrt_close(pi + 0);
			if (prev_rpi != 0)
			{
				if (dup2(prev_rpi, 0) == -1 && close_all(pi, prev_rpi))
					exit(1);
				smrt_close(&prev_rpi);
			}
			if (cmds[i + 1] != NULL)
			{
				if (dup2(pi[1], 1) == -1 && close_all(pi, prev_rpi))
					exit(1);
				smrt_close(pi + 1);
			}
			if (execvp(cmds[i][0], cmds[i]) == -1 && close_all(pi, prev_rpi))
				exit(1);
		}
	
		if (pid != 0)
		{
			if (pi[1] != 0)
				smrt_close(pi + 1);
			if (prev_rpi != 0)
				smrt_close(&prev_rpi);
			if (wait(&wstatus) == -1 || WEXITSTATUS(wstatus) == 1)
				return (close_all(pi, prev_rpi), 1);
		}
	}
	return (0);
}

