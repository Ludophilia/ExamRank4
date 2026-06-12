/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 01:11:36 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/12 21:41:31 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdarg.h>

// typedef enum e_fflg
// {
// 	PI0 = (1 << 0),
// 	PI1 = (1 << 1),
// 	LPI0 = (1 << 2)
// }	t_fflg;

// int vclose(int fd, ...)
// {
// 	va_list		args;
// 	int			cur_fd;

// 	va_start(args, fd);
// 	close(fd);
// 	while ((cur_fd = va_arg(args, int)) != 0)
// 		close(cur_fd);
// 	va_end(args);
// 	return (0);
// }

// int	close_mult(int fd_flgs, int pi[3], int i, char ***cmds)
// {
// 	if ((fd_flgs & PI0) && cmds[i + 1])
// 		close(pi[0]);
// 	if ((fd_flgs & PI1) && cmds[i + 1])
// 		close(pi[1]);
// 	if ((fd_flgs & LPI0) && i > 0)
// 		close(pi[2]);
// 	return (0);
// }

#include <stdio.h>

int	picoshell(char **cmds[])
{
	int		wstatus, pi[2], lpi0; 
	pid_t	pid;


	for (int i = 0; cmds[i]; i++)
	{
		if (cmds[i + 1])
			pipe(pi);

		pid = fork();
		if (pid == 0)
		{
			if (i > 0)
			{
				dup2(lpi0, 0);
				close(lpi0);
			}
			if (cmds[i + 1])
			{
				close(pi[0]);

				dup2(pi[1], 1);
				close(pi[1]);				
			}
			execvp(cmds[i][0], cmds[i]);
		}
		if (cmds[i + 1])
		{
			close(pi[1]);
		}
		if (i > 0)
		{
			close(lpi0);
		}
		wait(&wstatus); // NO !!!!!!!
		lpi0 = pi[0];
	}
	return (0);
}

// int	picoshell(char **cmds[])
// {
// 	int		pi[2], lpi0; // wstatus, 
// 	pid_t	pid;


// 	for (int i = 0; cmds[i]; i++)
// 	{
// 		if (cmds[i + 1])
// 			pipe(pi);

// 		pid = fork();
// 		if (pid == 0)
// 		{
// 			dup2(1, 42);

// 			if (i > 0)
// 			{
// 				dup2(lpi0, 0);
// 				close(lpi0);
// 				dprintf(42, "\t[%i][%i] closing lpi0 (%i) in child...\n", getpid(), i, lpi0); fflush(NULL);
// 			}
// 			if (cmds[i + 1])
// 			{
// 				close(pi[0]);
// 				dprintf(42, "\t[%i][%i] closing pi[0] (%i) in child...\n", getpid(), i, pi[0]); fflush(NULL);

// 				dup2(pi[1], 1);
// 				close(pi[1]);
// 				dprintf(42, "\t[%i][%i] closing pi[1] (%i) in child...\n", getpid(), i, pi[1]); fflush(NULL);
				
// 			}

// 			close(42);
// 			execvp(cmds[i][0], cmds[i]);
// 		}

// 		if (cmds[i + 1])
// 		{
// 			printf("[%i][%i] closing pi[1] (%i) in parent...\n", getpid(), i, pi[1]);
// 			close(pi[1]);
// 		}
// 		if (i > 0)
// 		{
// 			printf("[%i][%i] closing lpi0 (%i) in parent...\n", getpid(), i, lpi0);
// 			close(lpi0);
// 		}

// 		wait(NULL);
// 		printf("[%i][%i] DONE Waiting...\n\n", getpid(), i);

// 		fflush(NULL);
// 		lpi0 = pi[0];
// 	}
// 	return (0);
// }

// {---(0)>cmds[0]<(4)---pipe} 
// {---(3)>cmds[1]<(6)---pipe} 
// {---(5)>cmds[2]...(1)}
// int	picoshell(char **cmds[])
// {
// 	int		wstatus, pi[2], lpi0;
// 	pid_t	pid;

// 	if (cmds == NULL)
// 		return (1);
// 	for (int i = 0; cmds[i]; i++)
// 	{

// 		if (cmds[i][0] == NULL || (cmds[i + 1] && pipe(pi) == -1))
// 			return (i && close(lpi0), 1);

// 		if ((pid = fork()) == -1)
// 			return (i && close(lpi0), cmds[i + 1] && (close(pi[0]), close(pi[1])), 1);

// 		if (pid == 0)
// 		{

// 			// printf("[%i] About to exec %s...\n", getpid(), cmds[i][0]); //
// 			// fflush(NULL);

// 			if (i > 0)
// 			{
// 				if (dup2(lpi0, 0) == -1)
// 					(void)(close(lpi0), cmds[i + 1] && (close(pi[0]), close(pi[1])), exit(1));
// 				if (close(lpi0) == -1)
// 					(void)(cmds[i + 1] && (close(pi[0]), close(pi[1])), exit(1));
// 				printf("\t[%i][%i] closing lpi0 (%i) in child...\n", getpid(), i, lpi0); fflush(NULL);
// 				// char buf[128];
// 				// int bytes;

// 				// while ((bytes = read(0, buf, 64)))
// 				// {
// 				// 	printf("%s", buf);
// 				// }
// 				// exit(1);
// 			}
// 			if (cmds[i + 1])
// 			{
// 				dup2(1, 42);
				
// 							
// 				if (close(pi[0]) == -1 || dup2(pi[1], 1) == -1) // if (dup2(pi[1], 1) == -1) // close(pi[0]) == -1 || 	
// 					(void)(close(pi[1]), exit(1));
// 				dprintf(42, "\t[%i][%i] closing pi[0] (%i) in child...\n", getpid(), i, pi[0]); fflush(NULL);
// 				if (close(pi[1]) == -1)
// 					exit(1);
// 				dprintf(42, "\t[%i][%i] closing pi[1] (%i) in child...\n", getpid(), i, pi[1]); fflush(NULL);
				
// 				close(42);
// 			}

// 			// ./build.sh && valgrind --track-fds=yes ./pico

// 			// Stuck at nl...
// 			// THere's an open fd.
// 			// when nl is the last command, it works... so my i > 0 
// 			// and cmds[i + 1] have to play a role...

			
// 			if (execvp(cmds[i][0], cmds[i]) == -1)
// 				exit(1);
// 		}

// 		// printf("[%i] cmds[i + 1] -> %i, i > 0 -> %i\n", getpid(), cmds[i + 1] != NULL, i > 0);
// 		// fflush(NULL);


// 		if (cmds[i + 1]
// 			&& printf("[%i][%i] closing pi[1] (%i) in parent...\n", getpid(), i, pi[1])
// 			&& close(pi[1]) == -1)
// 			return (i && close(lpi0), cmds[i + 1] && close(pi[0]), 1);
				

// 		if (i > 0
// 			&& printf("[%i][%i] closing lpi0 (%i) in parent...\n", getpid(), i, lpi0)
// 			&& close(lpi0) == -1)
// 			return (cmds[i + 1] && close(pi[0]), 1);
		
// 		// if (i == 1)
// 		// 	close(pi[0]);
// 	 //
// 		// (void)(printf("[%i] Waiting %i...\n", getpid(), i), fflush(NULL));
// 		if (wait(&wstatus) == -1 || WEXITSTATUS(wstatus) == 1)
// 			return (cmds[i + 1] && close(pi[0]), 1);
		
// 		printf("[%i][%i] DONE Waiting...\n\n", getpid(), i); //
// 		fflush(NULL); //
// 		lpi0 = pi[0];
// 	}
// 	return (0);
// }

// int	smrt_close(int *fd)
// {
// 	close(*fd);
// 	*fd = 0;
// 	return (1);
// }

// int close_all(int pi[2], int prev_rpi)
// {
// 	if (pi[0] != 0 && pi[0] != prev_rpi)
// 		close(pi[0]);
// 	if (prev_rpi != 0)
// 		close(prev_rpi);
// 	if (pi[1] != 0)
// 		close(pi[1]);
// 	return (1);
// }

// // printf("==%i== Refreshed: pi[0] -> %i; pi[1] -> %i; prev_rpi -> %i\n",
// // 	getpid(), pi[0], pi[1], prev_rpi);
// int	picoshell(char **cmds[])
// {
// 	int		wstatus, pi[2] = {0, 0}, prev_rpi = 0;
// 	pid_t	pid;

// 	if (cmds == NULL)
// 		return (1);

// 	// {cmds[0] <4---pipe} 
// 	// {---[3]>cmds[1]<6---pipe} 
// 	// {---[5]>cmds[2]...}

// 	for (int i = 0; cmds[i]; i++)
// 	{
// 		if (cmds[i] == NULL || cmds[i][0] == NULL)
// 			return (close_all(pi, prev_rpi), 1);

// 		prev_rpi = (i > 0 ? pi[0]: 0);
		
// 		if (cmds[i + 1] != NULL && pipe(pi) == -1 && close_all(pi, prev_rpi))
// 			return (1);

	
// 		pid = fork();
// 		if (pid == -1 && close_all(pi, prev_rpi))
// 			return (1);


// 		if (pid == 0)
// 		{
// 			if (pi[0] != 0 && pi[0] != prev_rpi)
// 				smrt_close(pi + 0);
// 			if (prev_rpi != 0)
// 			{
// 				if (dup2(prev_rpi, 0) == -1 && close_all(pi, prev_rpi))
// 					exit(1);
// 				smrt_close(&prev_rpi);
// 			}
// 			if (cmds[i + 1] != NULL)
// 			{
// 				if (dup2(pi[1], 1) == -1 && close_all(pi, prev_rpi))
// 					exit(1);
// 				smrt_close(pi + 1);
// 			}
// 			if (execvp(cmds[i][0], cmds[i]) == -1 && close_all(pi, prev_rpi))
// 				exit(1);
// 		}
	
// 		if (pid != 0)
// 		{
// 			if (pi[1] != 0)
// 				smrt_close(pi + 1);
// 			if (prev_rpi != 0)
// 				smrt_close(&prev_rpi);
// 			if (wait(&wstatus) == -1 || WEXITSTATUS(wstatus) == 1)
// 				return (close_all(pi, prev_rpi), 1);
// 		}
// 	}
// 	return (0);
// }

