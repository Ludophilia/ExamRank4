/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 20:29:21 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/06 16:03:12 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argo.h"

// RESSOURCES MANAGEMENT

int parse_value(json *dst, FILE *stream)
{
	int		c;

	c = peek(stream);
	if (c != '-' && !isdigit(c)
		&& c != '"'
		&& c != '{')
		return (unexpected(stream), -1);
	if ((c == '-' || isdigit(c)) && (dst->type = INTEGER)
		&& parse_integer(&dst->integer, stream) == -1)
		return (-1);
	else if (c == '"' && (dst->type = STRING)
		&& parse_string(&dst->string, stream) == -1)
		return (-1);
	else if (c == '{' && (dst->type = MAP)
		&& parse_map(&dst->map, stream) == -1)
		return (-1);
	return (0);
}

int	argo(json *dst, FILE *stream)
{
	if (parse_value(dst, stream) == -1)
		return (-1);
	return (1);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (1);

	// char 	*filename = argv[1];
	json	file;
	FILE	*stream = fopen(argv[1], "r");

	if (argo(&file, stream) != 1)
	{
		free_json(file);
		fclose(stream);
		return (1);
	}

	serialize(file);
	printf("\n");

	free_json(file); // Personal addition
	fclose(stream);
}
