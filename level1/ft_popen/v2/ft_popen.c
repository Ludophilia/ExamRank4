/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 20:29:35 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/10 20:12:32 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

// TIP: const applies to the keyword on the left, or on the right if there's nothing.
int	ft_popen(char const *file, char *const argv[], char type)
{
	int		pp[2], r;
	pid_t	pid;

	if (((r = (type == 'r')) || type == 'w') == 0
		|| file == NULL
		|| argv == NULL || *argv == NULL
		|| pipe(pp) == -1)
		return (-1);
	if ((pid = fork()) == -1)
		return (close(pp[0]), close(pp[1]), -1);
	if (pid == 0 
		&& ((r? close(pp[0]): close(pp[1])) == -1
			|| (r? dup2(pp[1], 1): dup2(pp[0], 0)) == -1
			|| (r? close(pp[1]): close(pp[0])) == -1
			|| execvp(file, argv) == -1))
	{
		(void)(close(pp[0]), close(pp[1]));
		exit(1);
	}
	if ((r? close(pp[1]): close(pp[0])) == -1)
		return (close(pp[0]), close(pp[1]), -1);
	return (r? pp[0]: pp[1]);
}
