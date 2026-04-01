/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 22:19:49 by jegerman          #+#    #+#             */
/*   Updated: 2026/04/02 00:25:47 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

int	vbc(char *expr)
{
	
}

int	main(int argc, char **argv)
{
	if (argv++ && --argc != 1)
		return (1);
	// WHAT? Another Recursive Descent Parser?
	// I didn't even had the opportunity to validate it in minishell...
	// Anyway...
	return (0);
}
