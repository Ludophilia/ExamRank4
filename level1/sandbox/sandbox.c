/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 20:40:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/04/01 21:24:14 by jegerman         ###   ########.fr       */
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

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	pid;
	int		wstatus;

	if (f == NULL || timeout == 0)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		alarm(timeout);
		f();
		exit(0);
	}
	if (waitpid(pid, &wstatus, 0) == -1)
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

void	test_sigabort(void) { abort(); }

void	test_sigkill(void) { kill(getpid(), SIGKILL); }

void	test_sigfpe(void) { int zero = 0, wtf = 42 / zero; (void)wtf; }

void	test_sigterm(void) { raise(SIGTERM); }

void	test_sigint(void) { raise(SIGINT); }

void	test_sigquit(void) { raise(SIGQUIT); }

void	test_sigpipe(void) { int fds[2]; pipe(fds); close(fds[0]); 
	write(fds[1], "42", 2); close(fds[1]); }

void	test_segfault(void) { *(int *)0 = 42; }

void	test_exit_42(void) { exit(42); }

void	test_exit_0(void) { exit(0); }

void	test_timeout(void) { pause(); }

int	main(void)
{
	printf("-> sandbox exit: %i\n", sandbox(test_timeout, 1, true));
}
