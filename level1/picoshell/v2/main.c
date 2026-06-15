/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:19:36 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/15 18:55:50 by jegerman         ###   ########.fr       */
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
	int		picoshell(char **[]);

	cmds1 = (char **[])
	{
		// (char *[]){"last", NULL},
		// (char *[]){"sleep", "7", NULL},
		// (char *[]){"sleep", "5", NULL},

		(char *[]){"echo", "'squalala'", NULL},
		(char *[]){"cat", NULL},
		(char *[]){"sed", "s/a/b/g", NULL},

		// (char *[]){"sed", "'s/a/b/g'", NULL},

		// (char *[]){"picoshell", NULL},

		
		// (char *[]){"nl", NULL},
		// (char *[]){"nl", NULL},
		// (char *[]){"nl", NULL},
		// (char *[]){"head", "-19", NULL},
		// (char *[]){"nl", NULL},

		// (char *[]){"tac", NULL},
		// (char *[]){"tac", NULL},
		// (char *[]){"cat", NULL},
		// (char *[]){"cat", NULL},
		// (char *[]){"cat", NULL},
		// (char *[]){"cat", NULL},

		// // (char *[]){"rev", NULL},
		// (char *[]){"tee", NULL},
		// (char *[]){"rev", NULL},
		// (char *[]){"tac", NULL},
		// (char *[]){"sed", "s/a/b/g", NULL},
		NULL
	};
	// cmds1 = (char **[])
	// {
	// 	(char *[]){"/bin/ls", NULL},
	// 	// (char *[]){"last", NULL}, // (0: W0, R0) (4, 3)
	// 	(char *[]){"nl", NULL},  // (1: lpi0 -> R0, W1, R1) (3, 4, 5)
	// 	(char *[]){"head", "-5", NULL}, // (2: lpi0 -> R1)

	// 	(char *[]){"tac", NULL},
	// 	(char *[]){"rev", NULL},
	// 	(char *[]){"tee", NULL},
	// 	(char *[]){"rev", NULL},
	//  (char *[]){"tac", NULL},
	// 	(char *[]){"sed", "s/a/b/g", NULL},
	// 	NULL
	// };
	cmds2 = (char **[])
	{
		(char *[]){"/bin/ls", NULL},
		(char *[]){"/usr/bin/grep", "picoshell", NULL},
		(char *[]){"rev", NULL},
		(char *[]){"rev", NULL},
		(char *[]){"nl", NULL},
		NULL
	};
	if (picoshell(cmds1) == 1)
		return (1);

	(void)cmds1;
	(void)cmds2;

	return (0);
}
