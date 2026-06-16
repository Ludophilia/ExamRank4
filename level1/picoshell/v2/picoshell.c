/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 01:11:36 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/16 19:26:02 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
// #include <stdio.h>

#define CL(FLG) (close_all((FLG), &f, i, cmds))
#define CLEX(FLG, VAL) ((void)(CL((FLG)), exit((VAL))))

typedef enum e_flg
{
	P0 = (1 << 0), P1 = (1 << 1), P0L = (1 << 2),
	P0P1 = (P0 | P1),
	PALL = (P0 | P1 | P0L),
}	t_flg;

typedef struct s_fds
{
	int		pi[2];
	int		lpi0;
}	t_fds;

int	close_all(int flags, t_fds *f, int i, char **cmds[])
{
	if ((flags & P0L) && i > 0)
		close(f->lpi0);
	if ((flags & P0) && cmds[i + 1])
		close(f->pi[0]);
	if ((flags & P1) && cmds[i + 1])
		close(f->pi[1]);
	return (0);
}

int	picoshell(char **cmds[])
{
	t_fds		f;
	pid_t		pid;
	int			wstatus, werrors = 0;

	for (int i = 0; cmds[i]; i++)
	{
		if (cmds[i + 1] && pipe(f.pi) == -1)
			return (CL(P0L), 1);
		pid = fork();
		if (pid == -1)
			return (CL(PALL), 1);
		if (pid == 0)
		{
			if (i > 0 && dup2(f.lpi0, 0) == -1)
				CLEX(PALL, 1);
			if (i > 0 && close(f.lpi0) == -1)
				CLEX(P0P1, 1);
			if (cmds[i + 1] && close(f.pi[0]) == -1)
				CLEX(P1, 1);
			if (cmds[i + 1] && dup2(f.pi[1], 1) == -1)
				CLEX(P1, 1);
			if (cmds[i + 1] && close(f.pi[1]) == -1)
				exit(1);
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (i > 0 && close(f.lpi0) == -1)
			return (CL(P0), 1);
		if (cmds[i + 1] && close(f.pi[1]) == -1)
			return (CL(P0 | P0L), 1);
		f.lpi0 = cmds[i + 1]? f.pi[0]: -1;
	}
	for (int j = 0; cmds[j]; j++)
		if (wait(&wstatus) == -1 || WEXITSTATUS(wstatus) == 1)
			werrors++;
	return (werrors? 1 : 0);
}
