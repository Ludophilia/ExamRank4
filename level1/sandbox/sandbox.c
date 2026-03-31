/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 20:40:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/03/31 01:37:18 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <errno.h>


// PARENT:
// 		- fork, waitpid, kill, alarm (?), printf
//		- WIFEXITED / WIFSIGNALED
// CHILD:
// 		- exit, strsignal, 

// I've barely touched signals (no minitalk, I didn't set them in minishell)
// so besides signal(nb, handler)... I don't know anything.

// This is pretty new:
// - strsignal, sigaction, sigaddset, sigemptyset, sigfillset, sigdelset, sigismember
//  

// - Will return 1 if f is nice, 0 if f is bad or -1 in case of an error 
// in your function.
// - A function is considered bad if it is terminated or stopped by a signal
// (segfault, abort...), if it exit with any other exit code than 
// 0 or if it times out.
int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	
}

int	main(void)
{
	
}
