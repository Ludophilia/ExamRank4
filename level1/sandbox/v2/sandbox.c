/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 20:40:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/19 22:05:27 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
// #include <errno.h>
#include <string.h>
#include <stdio.h>

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	pid;
	int		wstatus;

	if (f == NULL)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		printf("child pid: %i\n", getpid());
		alarm(timeout); // ARE YOU SURE ABOUT THAT?
		// signal(SIGALRM, SIG_IGN); // pwnd??? ;)
		f();
		exit(0);
	}
	printf("parent pid: %i\n", getpid());
	if (waitpid(pid, &wstatus, 0) == -1) // WUNTRACED
		return (-1);

	// without WUNTRACED, wait will never react to a signal that is stopped... 
    // if(waitpid(pid, &status, WUNTRACED) == -1) 
    // {
	// if (WIFSTOPPED(status)) 
	// 	printf("Children 've been stopped. Yeah, that sucks...\n");
		
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

