/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 20:28:20 by jegerman          #+#    #+#             */
/*   Updated: 2026/05/26 00:04:20 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argo.h"

// '1'
// => (json){.type = INTEGER, .integer = 1};
int	parse_integer(json *dst, FILE *stream)
{
	int		nbr, c, sign;

	nbr = 0;
	sign = 1;
	dst->type = INTEGER;
	c = peek(stream);
	if (c == '-')
	{
		consume(stream);
		sign *= -1;
	}
	c = peek(stream);
	if (isdigit(c) == 0)
		return (unexpected(stream), -1);
	while (c != EOF && isdigit(c))
	{
		consume(stream);
		nbr = 10 * nbr + (c - '0');
		c = peek(stream);
	}
	dst->integer = (sign * nbr);
	return (0);
}

// ======================================================

// '"bonjour"'
// => (json){.type = STRING, .string = "bonjour"};
int	parse_string(json *dst, FILE *stream)
{
	char	buffer[512];
	int		i, c;

	dst->type = STRING;
	if (expect(stream, '"') == 0)
		return (unexpected(stream), -1);
	i = 0;
	c = peek(stream);
	while (c != EOF && c != '"')
	{
		if (c == '\\')
		{
			consume(stream);
			c = peek(stream);
			if (c != '\\' && c != '"')
				return (unexpected(stream), -1);
		}
		buffer[i++] = c;
		consume(stream);
		c = peek(stream);
	}
	buffer[i] = '\0';
	if (expect(stream, '"') == 0)
		return (unexpected(stream), -1);

	// 26/05: POORLY ALLOCATED, please try again. Weird bug...
	printf("i -> %i\n", i);
	if ((dst->string = malloc(i * sizeof(char))) == NULL)
		return (-1);
	int j = 0;
	while (buffer[j])
	{
		printf("[%i] %c\n", j, buffer[j]);
		dst->string[j] = buffer[j];
		printf("dst->string + [%i] -> %p\n", j, dst->string + j);
		j++;
	}
	// printf("j -> %i\n", j);
	dst->string[i] = 0;
	return (0);
}

// ======================================================

// $> echo -n '{"tomatoes":42,"potatoes":234}' | ./argo /dev/stdin | cat -e
// $> echo -n '{"recursion":{"recursion":{"recursion":{"recursion":"recursion"}}}}' | ./argo /dev/stdin | cat -e
// int	parse_map(json *dst, FILE *stream)
// {
// 	pair	*pairs, *tmp;
// 	int		c;
// 	int		size;

// 	dst->type = MAP;
// 	if (expect(stream, '{') == 0)
// 		return (-1);
// 	c = peek(stream);
// 	size = 0;
// 	while (c != EOF && c == '"') // c == '"' is start of a key... But what if there's none?
// 	{
// 		if (size == 0)
// 		{
// 			pairs = calloc(++size * sizeof(pair));
// 			if (pairs == NULL)
// 				return (-1);
// 		}
// 		else
// 		{
// 			tmp = realloc(pairs, ++size * sizeof(pair));
// 			if (tmp == NULL)
// 				return (free(pairs), -1); // 
// 			pairs = tmp;
// 		}

// 		pairs[size - 1].key = allocate_string(stream);
// 		if (pairs[size - 1].key == NULL)
// 			return (free(pairs), -1);

// 		if (expect(stream, ':') == 0)
// 			return ();
// 		parse_value(&pairs[size - 1].value, stream);
// 			// error

// 		c = peek(stream);
// 		// if (c == EOF)
// 			// error ? end ?

// 		if (c == ',')
// 		{
// 			consume(stream);
// 			c = peek(stream);
// 			// if (c == EOF)
// 				// error ? end ?
// 		}
// 	}
// 	expect(stream, '}');
// 	/// error ?

// 	dst->map.data = pairs;
// 	dst->map.size = size;

// 	return (0);
// }
