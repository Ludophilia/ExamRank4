/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 19:09:49 by jegerman          #+#    #+#             */
/*   Updated: 2026/07/10 20:30:42 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

int	bi_cd(char **cmd, int size, t_cnt *ct)
{
	ct->cmds =- 1;
	if (size != 2)
	{
		write(2, "error: cd: bad arguments\n", 25);
		return (-1);
	}
	if (chdir(cmd[1]) == -1)
	{
		write(2, "error: cd: cannot change directory to ", 38);
		write(2, cmd[1], ft_strlen(cmd[1]));
		write(2, "\n", 1);
		return (-1);
	}
	return (0);
}

int	create_cmd(char **toks, char ***cmd, t_cnt *ct)
{
	int		size;
	int		j;

	size = 0;
	while (toks[size]
		&& strncmp(toks[size], "|", 2)
		&& strncmp(toks[size], ";", 2))
		size++;
	*cmd = malloc((size + 1) * sizeof(char *));
	if (*cmd == NULL)
		return (-1);
	ct->cmds++;
	j = -1;
	while (++j < size)
		(*cmd)[j] = toks[j];
	(*cmd)[j] = 0;
	return (ct->toks = size);
}

int exec_cmd(char **cmd, t_cnt *ct, int *pi, char **envp)
{	
	pid_t	pid;

	if ((pid = fork()) == -1)
		return (-1);
	if (pid == 0)
	{
		if (ct->pi > 0 && close(pi[0]) == -1)
			CL1X(P012, 1);
		if (ct->cmds > 1 && (dup2(pi[2], 0) == -1 || close(pi[2]) == -1))
			CL1X(P1 | P2, 1);
		if (ct->pi > 0 && (dup2(pi[1], 1) == -1 || close(pi[1]) == -1))
			CL1X(P1, 1);
		execve(*cmd, cmd, envp);
		write(2, "error: cannot execute ", 22);
		write(2, *cmd, ft_strlen(*cmd));
		write(2, "\n", 1);
		free(cmd);
		exit(1);
	}
	if (ct->pi > 0 && close(pi[1]) == -1)
		return (CL1(P012), -1);
	if (ct->cmds > 1 && close(pi[2]) == -1)
		return (CL1(P0 | P2), -1);
	return (0);
}
