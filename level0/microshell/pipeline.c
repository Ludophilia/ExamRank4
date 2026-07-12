/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 19:06:39 by jegerman          #+#    #+#             */
/*   Updated: 2026/07/12 13:14:12 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

static int	wait_pipeline(t_cnt *ct)
{
	int		wstatus;
	int		wexv;
	int		fatal;
	int		error;

	fatal = 0;
	error = 0;
	for (int j = 0; j < ct->cmds; j++)
	{
		if (waitpid(-1, &wstatus, 0) == -1
			|| (wexv = WEXITSTATUS(wstatus)) == 1)
			fatal++;
		if (wexv == 2)
			error++;
	}
	if (fatal > 0)
		return (-1);
	if (error > 0)
		return (-2);
	return (0);
}

static int	dispatch_cmd(char **cmd, int *pi, char **envp, t_cnt *ct)
{
	int		is_bi;
	int		exv;
	
	is_bi = !strncmp(*cmd, "cd", 3);
	if (is_bi && (exv = bi_cd(cmd, ct->toks, ct)) < 0)
		return (CL1(P012), exv);
	if (!is_bi && (exv = exec_cmd(cmd, ct, pi, envp)) < 0)
		return (exv);
	return (0);
}

int	exec_pipeline(char **toks, char **envp, int *toks_usd)
{
	char	**cmd;
	int		pi[3];
	t_cnt	ct;
	int		i;
	int		exv;

	i = 0;
	ct = (t_cnt){.pi = 0, .cmds = 0};
	while (toks[i] && strncmp(toks[i], ";", 2))
	{
		if (create_cmd(toks + i, &cmd, &ct) == -1)
			return (CL0(P2), -1);
		i += ct.toks;
		if (toks[i] && !strncmp(toks[i], "|", 2) && ft_pipe(pi, &i, &ct) == -1)
			return (FR(cmd), CL0(P2), -1);
		if ((exv = dispatch_cmd(cmd, pi, envp, &ct)) < 0)
			return (FR(cmd), exv);
		free(cmd);
		pi[2] = ct.pi > 0 ? (ct.pi--, pi[0]) : -1;
	}
	if ((exv = wait_pipeline(&ct)) < 0)
		return (exv);
	return (*toks_usd = i);
}
