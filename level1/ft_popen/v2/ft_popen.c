/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 20:29:35 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/09 19:48:17 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

// const applies to the keyword on the left, or in the right if there's nothing.
int	ft_popen(char const *file, char *const argv[], char type)
{
	int		pp[2], type_r;
	pid_t	pid;

	if (((type_r = (type == 'r')) || type == 'w') == 0
		|| file == NULL
		|| argv == NULL || *argv == NULL
		|| pipe(pp) == -1)
		return (-1);
	if ((pid = fork()) == -1)
		return (close(pp[0]), close(pp[1]), -1);
	if (pid == 0)
	{
		if ((type_r? close(pp[0]): close(pp[1])) == -1
			|| (type_r? dup2(pp[1], 1): dup2(pp[0], 0)) == -1
			|| (type_r? close(pp[1]): close(pp[0])) == -1
			|| execvp(file, argv) == -1)
		{
			(void)(close(pp[0]), close(pp[1]));
			exit(1);
		}
	}
	if ((type_r? close(pp[1]): close(pp[0])) == -1)
		return (close(pp[0]), close(pp[1]), -1);
	return (type_r? pp[0]: pp[1]);
}

// int	ft_popen(char const *file, char *const argv[], char type)
// {
// 	int		ipp[2];
// 	int		opp[2];
// 	pid_t	pid;

// 	if ((type != 'r' && type != 'w')
// 		|| file == NULL
// 		|| argv == NULL || *argv == NULL
// 		|| pipe(ipp) == -1
// 		|| (pipe(opp) == -1 && close_all(ipp, ipp + 1, 0)))
// 		return (-1);
// 	pid = fork();
// 	if (pid == -1 && close_all(ipp, ipp + 1, opp, opp + 1, 0))
// 		return (-1);
// 	if (pid == 0)
// 	{
// 		if (close_all(ipp + 1, opp, 0) != 1
// 			|| (type == 'w' && dup2(ipp[0], 0) == -1)
// 			|| (type == 'r' && dup2(opp[1], 1) == -1))
// 		{
// 			close_all(ipp, opp + 1, 0);
// 			exit(1);
// 		}
// 		if (close_all(ipp, opp + 1, 0) && execvp(file, argv) == -1)
// 			exit(1);
// 	}
// 	if (type == 'r' && close_all(ipp, ipp + 1, opp + 1, 0))
// 		return (opp[0]);
// 	if (type == 'w' && close_all(ipp, opp, opp + 1, 0))
// 		return (ipp[1]);
// 	return (close_all(ipp, ipp + 1, opp, opp + 1, 0), -1);
// }
