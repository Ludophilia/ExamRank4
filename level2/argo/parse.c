/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 20:28:20 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/06 16:07:25 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argo.h"

// '1'
// => (json){.type = INTEGER, .integer = 1};
int	parse_integer(int *integer, FILE *stream)
{
	int		nbr, c, sign;

	nbr = 0;
	sign = 1;
	if ((c = peek(stream)) == '-')
	{
		accept(stream, c);
		sign = -1;
	}
	c = peek(stream);
	if (isdigit(c) == 0)
		return (unexpected(stream), -1);
	while (c != EOF && isdigit(c))
	{
		accept(stream, c);
		nbr = 10 * nbr + (c - '0');
		c = peek(stream);
	}
	*integer = (sign * nbr);
	return (0);
}

// ======================================================

// '""'
// '"bonjour"'
// => (json){.type = STRING, .string = "bonjour"};
int	parse_string(char **string, FILE *stream)
{
	char	buffer[512];
	int		i, c, f;

	f = 0;
	if (expect(stream, '"') == 0)
		return (-1);
	i = 0;
	c = peek(stream);
	while (c != EOF && c != '"')
	{
		if (c == '\\')
		{
			accept(stream, c);
			c = peek(stream);
			if (c != '\\' && c != '"' && ++f)
				break ;
		}
		buffer[i++] = c;
		accept(stream, c);
		c = peek(stream);
	}
	buffer[i] = '\0';
	if ((*string = calloc((i + 1), sizeof(char))) == NULL
		|| f != 0)
		return (f ? (unexpected(stream), -1) : -1);
	for (int j = 0; buffer[j]; j++)
		(*string)[j] = buffer[j];
	if (expect(stream, '"') == 0)
		return (-1);
	return (0);
}

// ======================================================

// $> echo -n '{}' | ./argo /dev/stdin | cat -e

// $> echo -n '{"tomatoes":42}' | ./argo /dev/stdin | cat -e
// $> echo -n '{"tomatoes":1,"potatoes":"out-of-stock"}'| ./argo /dev/stdin | cat -e
// $> echo -n '{"recursion":{"recursion":{"recursion":{"recursion":"recursion"}}}}' | ./argo /dev/stdin | cat -e
int	parse_map(map *map, FILE *stream)
{
	pair	*tmp, *curr;
	int		c;

	if (expect(stream, '{') == 0)
		return (-1);
	map->data = NULL;
	map->size = 0;
	c = peek(stream);
	while (c != EOF && c != '}')
	{
		tmp = realloc(map->data, ++map->size * sizeof(pair));
		if (tmp == NULL)
			return (--map->size, -1);
		map->data = tmp;
		curr = &map->data[map->size - 1];
		curr->key = NULL;
		curr->value.type = 0;
		if (parse_string(&curr->key, stream) == -1
			|| expect(stream, ':') == 0
			|| parse_value(&curr->value, stream) == -1)
			return (-1);
		if ((c = peek(stream)) == ',')
		{
			accept(stream, c);
			if ((c = peek(stream)) != '"')
				return (unexpected(stream), -1);
		}
	}
	if (expect(stream, '}') == -1)
		return (-1);
	return (0);
}
