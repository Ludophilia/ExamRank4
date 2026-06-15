/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 01:11:36 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/15 23:06:39 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

typedef enum e_flg
{
	P0 = (1 << 0), P1 = (1 << 1), PL = (1 << 2),
	P01 = (P0 | P1),
	ALL = (P0 | P1 | PL),	
}	t_flg;

typedef struct s_fds
{
	int		pi[2];
	int		lpi0;
}	t_fds;

int	close_all(int flags, t_fds *f, int i, char **cmds[])
{
	if ((flags & PL) && (i > 0))
		close(f->lpi0);
	if ((flags & P0) && cmds[i + 1])
		close(f->pi[0]);
	if ((flags & P1) && cmds[i + 1])
		close(f->pi[1]);
	return (0);
}
#define CL(FLG) (close_all((FLG), &f, i, cmds))
#define EX(FLG, VAL) ((void)(CL((FLG)), exit((VAL))))

int	picoshell(char **cmds[])
{
	t_fds		f;
	pid_t		pid;
	// // int		wstatus; // No error management right now. 

	for (int i = 0; cmds[i]; i++)
	{
		if (cmds[i + 1] && pipe(f.pi) == -1)
			return (CL(PL), 1);
		pid = fork();
		if (pid == -1)
			return (CL(ALL), 1);
		if (pid == 0)
		{
			if (i > 0 && dup2(f.lpi0, 0) == -1)
				EX(ALL, 1);
			if (i > 0 && close(f.lpi0) == -1)
				EX(P01, 1);
			if (cmds[i + 1] && close(f.pi[0]) == -1)
				EX(P1, 1);
			if (cmds[i + 1] && dup2(f.pi[1], 1) == -1)
				EX(P1, 1);
			if (cmds[i + 1] && close(f.pi[1]) == -1)
				exit(1);
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}


		//15/06: I'm not over yet...
		if (cmds[i + 1])
		{
			close(f.pi[1]);
		}
		if (i > 0)
		{
			close(f.lpi0);
		}


		f.lpi0 = (cmds[i + 1]? f.pi[0]: -1);
	}

	for (int i = 0; cmds[i]; i++)
		wait(NULL); // No error management right now. 
	
	return (0);
}

// int	picoshell(char **cmds[])
// {
// 	int		pi[2], lpi0;
// 	// int		wstatus; // No error management right now. 
// 	pid_t	pid;


// 	for (int i = 0; cmds[i]; i++)
// 	{
// 		if (cmds[i + 1] && pipe(pi) == -1)
// 			return (1);
// 		if ((pid = fork()) == -1)
// 			return (1);
// 		if (pid == 0)
// 		{
// 			if (i > 0)
// 			{
// 				dup2(lpi0, 0);
// 				close(lpi0);
// 			}
// 			if (cmds[i + 1])
// 			{
// 				close(pi[0]);
// 				dup2(pi[1], 1);
// 				close(pi[1]);
// 			}
// 			execvp(cmds[i][0], cmds[i]);
// 			exit(1);
// 		}



// 		if (cmds[i + 1])
// 		{
// 			close(pi[1]);
// 		}
// 		if (i > 0)
// 		{
// 			close(lpi0);
// 		}


// 		lpi0 = cmds[i + 1]? pi[0]: -1;
// 	}

// 	for (int i = 0; cmds[i]; i++)
// 		wait(NULL); // No error management right now. 
	
// 	return (0);
// }
