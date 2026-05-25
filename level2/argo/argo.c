/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 20:29:21 by jegerman          #+#    #+#             */
/*   Updated: 2026/05/25 22:01:56 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argo.h"

// RESSOURCES MANAGEMENT

// void	free_pairs(pair *pairs, int size)
// {
// 	for (int i = 0; i < size; i++)
// 	{
// 		pairs[i].key;
// 		pairs[i].value;
// 	}
// 	free(pairs);
// }

int parse_value(json *dst, FILE *stream)
{
	int		c;

	c = peek(stream);
	if ((c == '-' || isdigit(c))
		&& parse_integer(dst, stream) == -1)
		return (-1);
	// else if (c == '"'
	// 	&& parse_string(dst, stream) == -1)
	// 	return (-1);
	// else if (c == '{'
	// 	&& parse_map(dst, stream) == -1)
	// 	return (-1);
	return (0);
}

// 20/04: Don't forget to check memory leaks...
int	argo(json *dst, FILE *stream)
{
	if (parse_value(dst, stream) == -1)
		return (-1);
	return (1);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return 1;

	// char 	*filename = argv[1];
	FILE 	*stream = fopen(argv[1], "r");
	json	file;

	if (argo (&file, stream) != 1)
	{
		free_json(file);
		fclose(stream);
		return 1;
	}

	serialize(file);
	printf("\n");
	free_json(file); // Personal addition
	fclose(stream);
}
