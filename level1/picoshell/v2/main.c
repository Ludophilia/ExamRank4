/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:19:36 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/08 19:20:07 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

// ./picoshell /bin/ls "|" /usr/bin/grep picoshell
// picoshell
// ./picoshell echo 'squalala' "|" cat "|" sed 's/a/b/g'
// squblblb
int	main(void)
{
	// The argv array will be transformed to a char	*** like those below
	// following pipes position.
	char	***cmds1, ***cmds2;

	cmds1 = (char **[])
	{
		(char *[]){"last", NULL},
		(char *[]){"nl", NULL},
		(char *[]){"head", "-5", NULL},
		(char *[]){"tac", NULL},
		(char *[]){"rev", NULL},
		(char *[]){"tee", NULL},
		(char *[]){"rev", NULL},
		(char *[]){"sed", "s/a/b/g", NULL},
		NULL
	};
	cmds2 = (char **[])
	{
		(char *[]){"/bin/ls", NULL},
		(char *[]){"/usr/bin/grep", "picoshell", NULL},
		(char *[]){"rev", NULL},
		NULL
	};
	if (picoshell(cmds1) == 1)
		return (1);

	(void)cmds1;
	(void)cmds2;

	return (0);
}