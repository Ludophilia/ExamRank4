/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 16:00:16 by jegerman          #+#    #+#             */
/*   Updated: 2026/07/11 20:43:36 by jegerman         ###   ########.fr       */
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
	int		exv;

	i = 0;
	while (toks[i])
	{
		if ((exv = exec_pipeline(toks + i, envp, &ct.toks)) < 0)
			return (exv);
		i += ct.toks;
		if (toks[i] && !strncmp(toks[i], ";", 2))
			i++;
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	int		exv;

	if (argc == 1)
		return (1);
	if ((exv = microshell(++argv, envp)) == -1)
		return (fatal(), 2);
	if (exv == -2)
		return (3);
	return (0);
}
