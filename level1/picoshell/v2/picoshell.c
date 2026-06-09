/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 01:11:36 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/09 21:29:13 by jegerman         ###   ########.fr       */
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

	// {---(0)>cmds[0]<(4)---pipe} 
	// {---(3)>cmds[1]<(6)---pipe} 
	// {---(5)>cmds[2]...(1)}

	// TWO FDS... That's all there is to manage.
	//		- But there's more.
	//			- Those two fds have to be managed on two levels.

	for (int i = 0; cmds[i]; i++)
	{
		// In parent

		if (cmds[i][0] == NULL)
		{
			// How much is i?
				// first or i == 0, no pipe have been opened yet, nothing to close, return (1);
				// last or cmds[i + 1] == NULL, there's only the read end to close.
				// else,
					// there's only the read end as well. No new pipe have been 
					// opened yet.
			return (close_all(pi, prev_rpi), 1); // BULLSHIT. Poorly thought out...
		}

		prev_rpi = (i > 0 ? pi[0]: 0);
		
		if (cmds[i + 1] != NULL && pipe(pi) == -1 && close_all(pi, prev_rpi))
			return (1);

	
		pid = fork();
		if (pid == -1 && close_all(pi, prev_rpi))
			return (1);


		// In children
		if (pid == 0)
		{

			// The problem (most of the time) in children:
				// - One opened pipe (both directions)
				// - One reference to a previous pipe.

			// always close read end of pipe (if exists) you definitely don't need 
			if (pi[0] != 0 && pi[0] != prev_rpi)
				smrt_close(pi + 0);
			

			if (prev_rpi != 0)
			{
				// What does remain at that point? Not ALL...
				if (dup2(prev_rpi, 0) == -1 && close_all(pi, prev_rpi))
					exit(1);
				smrt_close(&prev_rpi); // close previous read end of the pipe (if exists)
			}
			if (cmds[i + 1] != NULL)
			{
				// What does remain at that point? Not ALL...
				if (dup2(pi[1], 1) == -1 && close_all(pi, prev_rpi))
					exit(1);
				smrt_close(pi + 1); // close write end of pipe (if exists) once dup2'd 
			}

		
			// What's still open at that point? Nothing. So why close_all?
			if (execvp(cmds[i][0], cmds[i]) == -1)
				exit(1);

			// We'll never see past this... EITHER execvp suceed and the prog
			// is executed, OR execvp fail, and exit(1) is executed.
		}

		// In parent (again)

		// Always close the write end of the pipe after use in children.
		if (pi[1] != 0)
			smrt_close(pi + 1);
		// Leave the read end open. Close the PREVIOUS read end.
		if (prev_rpi != 0)
			smrt_close(&prev_rpi);

		// Close_all? Besides the read end (if exists), what do you even close?
		// At that level, there's only the read end (if exits) that's still there.
		if (wait(&wstatus) == -1 || WEXITSTATUS(wstatus) == 1)
			return (close_all(pi, prev_rpi), 1); // Bullshit, poorly thought out...
	}
	return (0);
}

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

