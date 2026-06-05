/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 20:28:20 by jegerman          #+#    #+#             */
/*   Updated: 2026/06/05 22:49:53 by jegerman         ###   ########.fr       */
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
	c = peek(stream);
	if (c == '-')
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

// 5/06: I want something akin to int	parse_string(char **string, FILE *stream)
// pass the structure directly, not the whole json.

// int	parse_map(json *dst, FILE *stream)
// {
// 	pair	*tmp;
// 	int		c;

// 	dst->type = MAP;
// 	if (expect(stream, '{') == 0)
// 		return (-1);

// 	// 30/05: What are we trying to do?

// 	// After {, that's LL(1). Character after character, fill the json structure,
// 	// here, as a map: a 'pair *' data and a 'int' size. Each 'pair' are a
// 	// 'char *' key and 'json' value;

// 	c = peek(stream);


// 	dst->map.size = 0;
// 	while (c != EOF) // && c == '"') // c == '"' is start of a key... But what if there's none?
// 	{
// 		// 30/05: Dynamic array management.
// 		if (dst->map.size == 0)
// 		{
// 			dst->map.data = calloc(++dst->map.size, sizeof(pair));
// 			if (dst->map.data == NULL)
// 				return (dst->map.size = 0, -1);
// 		}
// 		else
// 		{
// 			tmp = realloc(dst->map.data, ++dst->map.size * sizeof(pair));
// 			if (tmp == NULL)
// 				return (dst->map.size--, -1);
// 			dst->map.data = tmp;
// 		}

// 		// Parsing key

		
// 		// Now you can use parse string with the key as a whoa.
// 		pairs[size - 1].key = allocate_string(stream);
// 		if (pairs[size - 1].key == NULL)
// 			return (free(pairs), -1);

// 		// Parsing key value separator
// 		if (expect(stream, ':') == 0)
// 			return (-1);

// 		// Parsing value (integer, string or another map (recusion))	
// 		parse_value(&pairs[size - 1].value, stream);

// 		c = peek(stream);

// 		// Parsing pair separator if exists
// 		if (c == ',')
// 		{
// 			consume(stream);
// 			c = peek(stream);

// 		}
// 	}


// 	// end of map 
// 	if (expect(stream, '}') == -1)
// 		return (-1);

// 	return (0);
// }
