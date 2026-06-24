/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 20:40:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/24 23:14:49 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

int		g_sig_trackr;

void	sigalarm_handler(int signum)
{
	g_sig_trackr = signum;
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t				pid;
	int					wstatus;
	struct sigaction	sa;

	g_sig_trackr = 0;
	if (f == NULL
		|| (pid = fork()) == -1)
		return (-1);
	if (pid == 0)
	{
		f();
		exit(0);
	}
	alarm(timeout);
	sa.sa_handler = sigalarm_handler;
	sa.sa_flags = 0;
	if (sigfillset(&sa.sa_mask) == -1
		|| sigaction(SIGALRM, &sa, NULL) == -1
		|| (waitpid(pid, &wstatus, WUNTRACED) == -1 && g_sig_trackr == 0))
		return (-1);




	// 25/06: Please improve the structure
	// Why not move it to another function?
		
	if (g_sig_trackr)
	{
		kill(pid, SIGTERM);
		waitpid(pid, NULL, 0);
		return (0);
	}

	if (WIFSTOPPED(wstatus)) // Incomplete 
	{
		printf("Signal stopped\n"); //by %s\n", strsignal(WSTOPSIG(wstatus)));

		// WSTOPSIG(wstatus)
		//  printf("kill -> %i\n", kill(pid, SIGTERM));
		// printf("wait -> %i\n", waitpid(pid, NULL, 0));
		// printf("errno: %i, %s\n", errno, strerror(errno));
		return (0);
	}
		
	if (WIFEXITED(wstatus))
	{
		if (WEXITSTATUS(wstatus) == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return (1);
		}
		if (verbose)
			printf("Bad function: exited with code %i\n", WEXITSTATUS(wstatus));
		return (0);
	}
	if (WIFSIGNALED(wstatus))
	{
		if (verbose)
		{
			if (WTERMSIG(wstatus) == SIGALRM)
				printf("Bad function: timed out after %u seconds\n", timeout);
			else
				printf("Bad function: %s\n", strsignal(WTERMSIG(wstatus)));
		}
		return (0);
	}
	return (-1);
}

// int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
// {
// 	pid_t	pid;
// 	int		wstatus;

// 	if (f == NULL)
// 		return (-1);
// 	pid = fork();
// 	if (pid == -1)
// 		return (-1);
// 	if (pid == 0)
// 	{
// 		printf("child pid: %i\n", getpid());
// 		alarm(timeout); // ARE YOU SURE ABOUT THAT?
// 		// signal(SIGALRM, SIG_IGN); // pwnd??? ;)
// 		f();
// 		exit(0);
// 	}
// 	printf("parent pid: %i\n", getpid());
// 	if (waitpid(pid, &wstatus, 0) == -1) // WUNTRACED
// 		return (-1);

// 	// without WUNTRACED, wait will never react to a signal that is stopped... 
//     // if(waitpid(pid, &status, WUNTRACED) == -1) 
//     // {
// 	// if (WIFSTOPPED(status)) 
// 	// 	printf("Children 've been stopped. Yeah, that sucks...\n");
		
// 	if (WIFEXITED(wstatus))
// 	{
// 		if (WEXITSTATUS(wstatus) == 0)
// 		{
// 			if (verbose)
// 				printf("Nice function!\n");
// 			return (1);
// 		}
// 		if (verbose)
// 			printf("Bad function: exited with code %i\n", WEXITSTATUS(wstatus));
// 		return (0);
// 	}
// 	if (WIFSIGNALED(wstatus))
// 	{
// 		if (verbose)
// 		{
// 			if (WTERMSIG(wstatus) == SIGALRM)
// 				printf("Bad function: timed out after %u seconds\n", timeout);
// 			else
// 				printf("Bad function: %s\n", strsignal(WTERMSIG(wstatus)));
// 		}
// 		return (0);
// 	}
// 	return (-1);
// }
