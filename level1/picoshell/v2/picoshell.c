/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 01:11:36 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/15 18:25:16 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

int	picoshell(char **cmds[])
{
	int		pi[2], lpi0;
	// int		wstatus; // No error management right now. 
	pid_t	pid;


	for (int i = 0; cmds[i]; i++)
	{
		if (cmds[i + 1])
			pipe(pi);



		pid = fork();
		if (pid == 0)
		{
			if (i > 0)
			{
				dup2(lpi0, 0);
				close(lpi0);
			}
			if (cmds[i + 1])
			{
				close(pi[0]);
				dup2(pi[1], 1);
				close(pi[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}


		if (cmds[i + 1])
		{
			close(pi[1]);
		}
		if (i > 0)
		{
			close(lpi0);
		}


		lpi0 = cmds[i + 1]? pi[0]: -1;
	}

	for (int i = 0; cmds[i]; i++)
		wait(NULL); // No error management right now. 
	
	return (0);
}
