/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 20:40:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/04/01 02:18:30 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

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
		alarm(timeout);
		f(); // ???
		exit(0); // ???
	}
	if (waitpid(pid, &wstatus, NULL) == -1)
		return (-1);
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

// - Will return 1 if f is nice, 0 if f is bad or -1 in case of an error 
// in your function.
// - A function is considered bad if:
//	 	- it is terminated or stopped by a signal (segfault, abort...), 
// 		- it exits with any other exit code than 0
//  	- or it times out.

void	test_abort(void) { abort(); }

void	test_sigkill(void) { kill(getpid, SIGKILL); }

void	test_segfault(void) { *(int *)0 = 42; }

void	test_exit_42(void) { exit(42); }

void	test_exit_0(void) { exit(0); }

void	test_timeout(void) { pause(); }

int	main(void)
{
	sandbox(test_exit_42, 0, true);

	// assert.h 
}
