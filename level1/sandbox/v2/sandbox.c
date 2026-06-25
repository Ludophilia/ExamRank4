/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 20:40:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/25 17:07:28 by jegerman         ###   ########.fr       */
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

int	process_result(bool verbose, pid_t pid, int wstatus, unsigned int timeout)
{
	int		sig_nb, exit_nb;

	if (g_sig_trackr == SIGALRM)
	{
		if (kill(pid, SIGKILL) == -1 || waitpid(pid, NULL, 0) == -1)
			return (-1);
		if (verbose) // How to het rid of these repeated if verbose?
			printf("Bad function: timed out after %u seconds\n", timeout);
		return (0);
	}
	if (WIFEXITED(wstatus))
	{
		exit_nb = WEXITSTATUS(wstatus);
		if (exit_nb == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return (1);
		}
		if (verbose)
			printf("Bad function: exited with code %i\n", exit_nb);
		return (0);
	}
	if (WIFSTOPPED(wstatus) || WIFSIGNALED(wstatus))
	{
		sig_nb = WIFSTOPPED(wstatus)? WSTOPSIG(wstatus): WTERMSIG(wstatus);
		if (verbose)
			printf("Bad function: %s\n", strsignal(sig_nb));
		return (0);
	}
	return (-1);
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t				pid;
	int					wstatus;
	struct sigaction	sa;
	int					res_exit;

	g_sig_trackr = 0;
	if (f == NULL || (pid = fork()) == -1)
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
		|| sigaction(SIGALRM, &sa, NULL) == -1)
		return (-1);
	if (waitpid(pid, &wstatus, WUNTRACED) == -1 
		&& g_sig_trackr == 0)
		return (-1);
	res_exit = process_result(verbose, pid, wstatus, timeout);
	return (res_exit);
}
// int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
// {
// 	pid_t				pid;
// 	int					wstatus;
// 	struct sigaction	sa;

// 	g_sig_trackr = 0;
// 	if (f == NULL
// 		|| (pid = fork()) == -1)
// 		return (-1);
// 	if (pid == 0)
// 	{
// 		f();
// 		exit(0);
// 	}
// 	alarm(timeout);
// 	sa.sa_handler = sigalarm_handler;
// 	sa.sa_flags = 0;
// 	if (sigfillset(&sa.sa_mask) == -1
// 		|| sigaction(SIGALRM, &sa, NULL) == -1
// 		|| (waitpid(pid, &wstatus, WUNTRACED) == -1 && g_sig_trackr == 0))
// 		return (-1);

// 	// if it exit with any other exit code than 0 or if it times out.	
// 	if (g_sig_trackr)
// 	{
// 		kill(pid, SIGTERM);
// 		waitpid(pid, NULL, 0);
// 		if (verbose)
// 			printf("Bad function: timed out after %u seconds\n", timeout);
// 		return (0);
// 	}

// 	//  A function is considered bad if it is terminated or stopped by a signal
// 	if (WIFSTOPPED(wstatus)) // Incomplete 
// 	{
// 		if (verbose)
// 			printf("Bad function: %s\n", strsignal(WSTOPSIG(wstatus)));
// 		return (0); // will return 0 if f is bad 
// 	}
	
// 	// if it exit with any other exit code than 0 or if it times out.
// 	if (WIFEXITED(wstatus))
// 	{
// 		if (WEXITSTATUS(wstatus) == 0)
// 		{
// 			if (verbose)
// 				printf("Nice function!\n");
// 			return (1); // will return 1 if f is nice
// 		}
// 		if (verbose)
// 			printf("Bad function: exited with code %i\n", WEXITSTATUS(wstatus));
// 		return (0); // will return 0 if f is bad 
// 	}

// 	//  A function is considered bad if it is terminated or stopped by a signal
// 	if (WIFSIGNALED(wstatus))
// 	{
// 		if (verbose)
// 			printf("Bad function: %s\n", strsignal(WTERMSIG(wstatus)));
// 		return (0); // will return 0 if f is bad 
// 	}
// 	return (-1); // will return -1 in case of an if f is bad 
// }




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
