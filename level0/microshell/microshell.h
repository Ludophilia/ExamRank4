/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 19:03:27 by jegerman          #+#    #+#             */
/*   Updated: 2026/07/10 22:11:46 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <wait.h>
# include <string.h>

# define CL0(flg) close_all((flg), 0, pi, &ct)
# define CL1(flg) close_all((flg), 0, pi, ct)
# define CL1X(flg, ex) close_all((flg | EX), (ex), pi, ct)
# define FR free

typedef struct s_cnt
{
	int		cmds;
	int		pi;
	int		toks;
}	t_cnt;

typedef enum e_flg
{
	P0 = (1 << 0), P1 = (1 << 1), P2 = (1 << 2), EX = (1 << 3),
	P01 = (P0 | P1),
	P012 = (P0 | P1 | P2),
	ALL = (EX | P012),
}	t_flg;


int		ft_strlen(char *str);
int		ft_pipe(int *fds, int *i, t_cnt *ct);
int		fatal_err(void);
int		close_all(int flags, int exitval, int *pi, t_cnt *ct);

int		wait_pipeline(t_cnt *ct);
int		exec_pipeline(char **toks, char **envp, int *toks_usd);

int		bi_cd(char **cmd, int size, t_cnt *ct);
int		create_cmd(char **toks, char ***cmd, t_cnt *ct);
int		exec_cmd(char **cmd, t_cnt *ct, int *pi, char **envp);

int		microshell(char **toks, char **envp);
