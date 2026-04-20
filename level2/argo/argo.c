/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 20:29:21 by jegerman          #+#    #+#             */
/*   Updated: 2026/04/20 15:59:42 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argo.h"

int parse_value(json *dst, FILE *stream)
{
	int		c;

	if ((c = peek(stream)) == EOF)
		return (-1); // no leaks?
	if (isdigit(c)
		&& parse_integer(dst, stream) == -1)
		return (-1);
	else if (c == '"'
		&& parse_string(dst, stream) == -1)
		return (-1);
	else if (c == '{'
		&& parse_map(dst, stream) == -1)
		return (-1);
	return (0);
}

// '1'
// => (json){.type = INTEGER, .integer = 1};
int	parse_integer(json *dst, FILE *stream)
{
	int		nbr, c;

	printf("HERE\n");
	nbr = 0;
	dst->type = INTEGER;
	// EOF: end of file or error?
	if ((c = peek(stream)) == EOF)
		return (-1); // no leaks?
	while (isdigit(c))
	{
		printf()
		if (consume(stream) == EOF)
			return (-1);
		nbr = 10 * nbr + (c - '0');
		if ((c = peek(stream)) == EOF)
			return (-1);
	}
	dst->integer = nbr;
	return (0);
}

int	parse_chars(FILE *stream, char *buffer)
{
	int		c, i;

	i = 0;
	if ((c = peek(stream)) == EOF)
		return (-1); // no leaks ?
	while (c != '"')
	{
		if (c == '\\')
		{
			if (consume(stream) == EOF
				|| (c = peek(stream)) == EOF)
				return (-1);
			if (c != '\\' && c != '"')
				return (unexpected(stream), -1);
		}
		buffer[i++] = c;
		if (consume(stream) == EOF
			|| (c = peek(stream)) == EOF)
			return (-1);
	}
	buffer[i] = '\0';
	return (i + 1);
}

char	*allocate_string(FILE *stream)
{
	char	buffer[512], *string;
	int		len;

	if (expect(stream, '"') == 0
		|| (len = parse_chars(stream, buffer)) == -1
		|| expect(stream, '"') == 0)
		return (NULL); // no leaks ?
	if ((string = calloc(len, sizeof(char))) == NULL)
		return (NULL);
	for (int i = 0; buffer[i]; i++)
		string[i] = buffer[i];
	return (string);
}

// '"bonjour"'
// => (json){.type = STRING, .string = "bonjour"};
int	parse_string(json *dst, FILE *stream)
{
	dst->type = STRING;
	if ((dst->string = allocate_string(stream)) == NULL)
		return (-1);
	return (0);
}

// $> echo -n '{"tomatoes":42,"potatoes":234}' | ./argo /dev/stdin | cat -e
// $> echo -n '{"recursion":{"recursion":{"recursion":{"recursion":"recursion"}}}}' | ./argo /dev/stdin | cat -e
int	parse_map(json *dst, FILE *stream)
{
	pair	*pairs, *tmp;
	int		c;
	int		size;

	dst->type = MAP;
	if (expect(stream, '{') == 0)
		return (-1);

	c = peek(stream);
	// if (c == EOF)
		// error ? end ?
	size = 0;
	while (c == '"') // supposedy. c != EOF && c == '"'
	{
		if (size == 0)
			pairs = malloc(++size * sizeof(pair));
		else
		{
			tmp = realloc(pairs, ++size * sizeof(pair));
			// if (tmp == NULL)
				// error
			pairs = tmp;
		}

		pairs[size - 1].key = allocate_string(stream);
		// if (pairs[size - 1].key == NULL)
			// error
		expect(stream, ':');

		parse_value(&pairs[size - 1].value, stream);
			// error

		c = peek(stream);
		// if (c == EOF)
			// error ? end ?

		if (c == ',')
		{
			consume(stream);
			c = peek(stream);
			// if (c == EOF)
				// error ? end ?
		}
	}
	expect(stream, '}');
	/// error ?

	dst->map.data = pairs;
	dst->map.size = size;

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

	char 	*filename = argv[1];
	FILE 	*stream = fopen(filename, "r");
	json	file;

	if (argo (&file, stream) != 1)
	{
		free_json(file);
		return 1;
	}

	serialize(file);
	printf("\n");
}
