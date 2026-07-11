/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 19:06:03 by jegerman          #+#    #+#             */
/*   Updated: 2026/07/11 20:43:39 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	ft_pipe(int *fds, int *i, t_cnt *ct)
{
	*i += 1;
	ct->pi = 1;
	return (pipe(fds));
}

void	fatal(void)
{
	write(2, "error: fatal\n", 14);
}

int	close_all(int flags, int exitval, int *pi, t_cnt *ct)
{
	if ((flags & P2) && ct->cmds > 1)
		close(pi[2]); 
	if ((flags & P0) && ct->pi > 0)
		close(pi[0]);
	if ((flags & P1) && ct->pi > 0)
		close(pi[1]);
	if (flags & EX)
		exit(exitval);
	return (0);
}
