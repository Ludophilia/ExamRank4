/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 19:06:39 by jegerman          #+#    #+#             */
/*   Updated: 2026/07/10 21:00:52 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

int	wait_pipeline(t_cnt *ct)
{
	int	errors;
	int	wstatus;

	errors = 0;
	for (int j = 0; j < ct->cmds; j++)
		if (waitpid(-1, &wstatus, 0) == -1 || WEXITSTATUS(wstatus) == 1)
			errors++;
	if (errors > 0)
		return (-1);
	return (0);
}

int	exec_pipeline(char **toks, char **envp, int *toks_usd)
{
	char	**cmd;
	int		pi[3];
	t_cnt	ct;
	int		i;
	int		is_bi;

	i = 0;
	ct = (t_cnt ){.pi = 0, .cmds = 0};
	while (toks[i] && strncmp(toks[i], ";", 2))
	{
		if (create_cmd(toks + i, &cmd, &ct) == -1)
			return (CL0(P2), -1);
		i += ct.toks;
		if (toks[i] && !strncmp(toks[i], "|", 2) && ft_pipe(pi, &i, &ct) == -1)
			return (CL0(P2), -1);
		if ((is_bi = !strncmp(*cmd, "cd", 3)) && bi_cd(cmd, ct.toks, &ct) == -1)
			return (CL0(P012), -1);
		if (!is_bi && exec_cmd(cmd, &ct, pi, envp) == -1)
			return (-1);
		free(cmd);
		pi[2] = ct.pi > 0 ? (ct.pi--, pi[0]) : -1;
	}
	if (wait_pipeline(&ct) == -1)
		return (-1);
	return (*toks_usd = i);
}
