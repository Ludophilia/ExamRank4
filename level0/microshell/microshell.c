/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 16:00:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/07/10 23:08:19 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

/*
valgrind --track-fds=yes ./microshell /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell

valgrind --track-fds=yes ./microshell /usr/bin/last "|" /usr/bin/head -20 "|" /usr/bin/head "|" /usr/bin/nl "|" /usr/bin/tac ";"

valgrind --track-fds=yes ./microshell /usr/bin/sleep 10 "|" /usr/bin/echo a ";" /usr/bin/echo b

valgrind --track-fds=yes ./microshell /usr/bin/pwd ";" cd .. ";" /usr/bin/pwd

valgrind --track-fds=yes ./microshell /usr/bin/pwd ";" cd .. ";" /usr/bin/pwd ";" cd .. ";" /usr/bin/pwd ";" cd .. ";" /usr/bin/pwd 

valgrind --track-fds=yes ./microshell /usr/bin/echo a ";" /usr/bin/echo b
valgrind --track-fds=yes ./microshell /usr/bin/echo a ";"

valgrind --track-fds=yes ./microshell /usr/bin/nukeall 0

valgrind --track-fds=yes ./microshell cd

valgrind --track-fds=yes ./microshell cd 1 2 3
*/

int microshell(char **toks, char **envp)
{
	int		i;
	t_cnt	ct;

	i = 0;
	while (toks[i])
	{
		if (exec_pipeline(toks + i, envp, &ct.toks) == -1)
			return (-1);
		i += ct.toks;
		if (toks[i] && !strncmp(toks[i], ";", 2))
			i++;
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	// - If a system call, except execve and chdir, returns an error your program should immediatly print "error: fatal" in STDERR followed by a '\n' and the program should exit
	int exv;

	if (argc == 1)
		return (1);

	// 10/07: 	
	if ((exv = microshell(++argv, envp)) == -1)
		return (2);
	if (exv == -2)
		return (3);

	
	return (0);
}
