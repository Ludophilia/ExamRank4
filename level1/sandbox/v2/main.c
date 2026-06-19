/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 20:10:13 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/19 21:10:20 by jegerman         ###   ########.fr       */
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
	 write(fds[1], "42", 2); write(1, "Do you see it (spoiler: no)? \n", 15);  
	/* close(fds[1]); (useless) */ }

void	test_segfault(void) { *(int *)0 = 42; }

void	test_exit_42(void) { exit(42); }

void	test_exit_0(void) { exit(0); }

void	test_timeout(void) { pause(); }

void	test_sigstop(void) { raise(SIGSTOP); }

int	main(void)
{
	int	sandbox(void (*)(void), unsigned int, bool);

	printf("-> sandbox exit: %i\n", sandbox(test_sigstop, 4, true));
}
