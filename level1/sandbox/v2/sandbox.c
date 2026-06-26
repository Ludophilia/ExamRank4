/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 20:40:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/26 15:03:31 by jegerman         ###   ########.fr       */
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
		printf("Bad function: %s\n", strsignal(*(int *)arg));
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
