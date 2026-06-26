/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 20:10:13 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/26 15:07:36 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void	test_sigabort(void) { abort(); }

void	test_sigkill(void) { kill(getpid(), SIGKILL); }

void	test_sigfpe(void) { int zero = 0, wtf = 42 / zero; (void)wtf; }

void	test_sigterm(void) { raise(SIGTERM); }

void	test_sigint(void) { raise(SIGINT); }

void	test_sigquit(void) { raise(SIGQUIT); }

void	test_sigpipe(void) { int fds[2]; pipe(fds); close(fds[0]); 
	 write(fds[1], "42", 2); /* close(fds[1]); */ }

void	test_segfault(void) { *(int *)0 = 42; }

void	test_exit_42(void) { exit(42); }

void	test_exit_0(void) { exit(0); }

void	test_timeout(void) { pause(); }

void	test_evil_timeout(void) { signal(SIGALRM, SIG_IGN); pause(); }

void	test_sigstop(void) { raise(SIGSTOP); }

void	test_sigtstp(void) { raise(SIGTSTP); }


int	main(void)
{
	int	sandbox(void (*)(void), unsigned int, bool);

	printf("== TEST NICE ==\n");
	printf("(sandbox exit: %i)\n", sandbox(test_exit_0, 3, true));
	printf("== TEST EXIT ==\n");
	printf("(sandbox exit: %i)\n", sandbox(test_exit_42, 3, true));
	printf("== TEST TIMEOUT ==\n");
	printf("(sandbox exit: %i)\n", sandbox(test_timeout, 3, true));
	printf("== TEST EVIL TIMEOUT ==\n");
	printf("(sandbox exit: %i)\n", sandbox(test_evil_timeout, 5, true));
	printf("== TEST SIGSTOP ==\n");
	printf("(sandbox exit: %i)\n", sandbox(test_sigstop, 3, true));
	printf("== TEST SIGTSTP ==\n");
	printf("(sandbox exit: %i)\n", sandbox(test_sigtstp, 3, true));
	printf("== TEST SEGFAULT ==\n");
	printf("(sandbox exit: %i)\n", sandbox(test_segfault, 1, true));	
	printf("== TEST SIGABORT ==\n");
	printf("(sandbox exit: %i)\n", sandbox(test_sigabort, 1, true));
	printf("== TEST SIGFPE ==\n");
	printf("(sandbox exit: %i)\n", sandbox(test_sigfpe, 7, true));
	printf("== TEST SIGPIPE ==\n");
	printf("(sandbox exit: %i)\n", sandbox(test_sigpipe, 7, true));
}
