/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 20:40:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/25 23:02:50 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

int		g_sig_trackr;

enum e_exid
{ 
	NICE, BAD_EX, BAD_SIG, BAD_TMO
};

void	sigalarm_handler(int signum)
{
	g_sig_trackr = signum;
}

int	print_msg(int id, void *arg, bool vrb)
{
	if (!vrb)
		return (0);
	switch (id)
	{
	case NICE:
		printf("Nice function!\n");
		break;
	case BAD_EX:
		printf("Bad function: exited with code %i\n", *(int *)arg);
		break;
	case BAD_SIG:
		printf("Bad function: %s\n", *(char *)arg);
		break;
	case BAD_TMO:
		printf("Bad function: timed out after %u seconds\n", *(unsigned *)arg);		
	default:
		break;
	}
	return (0);
}

int	process_result(bool vrb, pid_t pid, int wst, unsigned tmo)
{
	int		sig_nb, exit_nb;

	if (g_sig_trackr == SIGALRM)
	{
		if (kill(pid, SIGKILL) == -1 || waitpid(pid, NULL, 0) == -1)
			return (wait(NULL), -1);
		print_msg(BAD_TMO, &tmo, vrb);
		return (0);
	}
	if (WIFEXITED(wst))
	{
		if ((exit_nb = WEXITSTATUS(wst)) == 0 && !print_msg(NICE, NULL, vrb))
			return (1);
		print_msg(BAD_EX, &exit_nb, vrb);
		return (0);
	}
	if (WIFSTOPPED(wst) || WIFSIGNALED(wst))
	{
		sig_nb = WIFSTOPPED(wst)? WSTOPSIG(wst): WTERMSIG(wst);
		print_msg(BAD_SIG, &sig_nb, vrb);
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
		|| sigaction(SIGALRM, &sa, NULL) == -1
		|| (waitpid(pid, &wstatus, WUNTRACED) == -1 && g_sig_trackr == 0))
		return (wait(NULL), -1);
	res_exit = process_result(verbose, pid, wstatus, timeout);
	return (res_exit);
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
