/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 20:29:21 by jegerman          #+#    #+#             */
/*   Updated: 2026/04/19 01:35:20 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argo.h"


/* Can you use the examples and show how the json is translated from its
javascript / rfc8259 form to its C form? 

=> From the struct definitions alone, I just can't see how those two come together... 

EXAMPLES:

'1'
=> (json){.type = INTEGER, .integer = 1};

'"bonjour"'
=> (json){.type = STRING, .string = "bonjour"};

'{"tomatoes":42,"potatoes":234}'
=> (json){
	.type = MAP,
	.map = (map){
		.data = (pair[]){
			(pair){.key="tomatoes", .value=(json){.type= INTEGER, .integer=42}},
			(pair){.key="potatoes", .value=(json){.type= INTEGER, .integer=234}}			
		},
		.size = 2
	}
}
=> size 2 because there's 1 comma in the JSON object

'{"recursion":{"recursion":{"recursion":{"recursion":"recursion"}}}}'
OR
'{
	"recursion":{
		"recursion":{
			"recursion": {
				"recursion":"recursion"
			}
		}
	}
}'
=> (json){
	.type = MAP,
	.map = (map){
		.data = (pair[]){
			(pair){
				.key="recursion",
				.value=(json){
					.type= MAP,
					.map=(map){
						.data = (pair[]){
							(pair){
								.key = "recursion",
								.value = (json){
									.type= MAP,
									.map=(map){
										.data = (pair[]){
											(pair){
												.key = "recursion",
												.value = (json){
													(3 levels deep... 1 to go)
												}
											}
										}
										.size = 1
									}
								}
							}
						}
						.size = 1
					}
				}
			},
		},
		.size = 1
	}
}
=> size 1 because there's 1 key, 0 comma in the JSON object
	
*/

/* Please now try to write the grammar... from the top level down to
the terminals...

What contains what basically, what are the terminals, where is the mutual 
recursion case that justifies RECURSIVE descent parsing...

Start from the terminals maybe?

value ::= integer | string | map
map ::= '{' (pair (',' pair)*)? '}'
pair ::= string ':' value

integer ::= digit+
string ::= '"' character* '"'

digit ::= '0' | ... | '9'
character ::= ascii<32,127>
ascii<32,127> ::=  ' ' | ... | DEL 

*/

// ########################################################################


int parse_value(json *dst, FILE *stream)
{
	// That's just the outline. Nothing works...
	int c = peek(stream);
	// if (c == EOF)
		// error ? end ?
	if (isdigit(c))
	{
		parse_integer(dst, stream);
		// error...
	}
	else if (c == '"')
	{
		parse_string(dst, stream);
	}
	else if (c == '{')
	{
		parse_map(dst, stream);
	}
	return (0);// ??? 
}

// ########################################################################


// How do you know a JSON should be allocated or not?
// We start from a json allocated on stack, or at least something we don't
// control...

// '1'
// => (json){.type = INTEGER, .integer = 1};
int	parse_integer(json *dst, FILE *stream)
{
	int	nbr, c;

	// Alt: fscanf(stream, "%d", &dst->integer); TEST IT ;)
	nbr = 0;
	dst->type = INTEGER;
	c = peek(stream);
	// if (c == EOF)
		// error ? end ?
	while (c != EOF && isdigit(c))
	{
		consume(stream);
		// error ?
		nbr = 10 * nbr + (c - '0');
		c = peek(stream);
		// if (c == EOF)
			// error ? end ?
	}
	dst->integer = nbr;
	return (0); // ???
}

// ########################################################################

int	parse_chars(FILE *stream, char *buffer)
{
	int		c, i;

	i = 0;
	c = peek(stream);
	// if (c == EOF)
		// error ? end ?
	while (c != EOF && c != '"')
	{
		if (c == '\\')
		{
			consume(stream);
			c = peek(stream);
			// if (d == EOF)
				// error ? end ?
			if (c != '\\' && c != '"')
				unexpected(stream); // return
			buffer[i++] = c;
			consume(stream);
		}
		else
		{
			buffer[i++] = c;
			consume(stream);
		}
		c = peek(stream);
		// if (c == EOF)
			// error ? end ?
	}
	buffer[i] = '\0';
	// Error management
	return (i + 1);
}

char	*allocate_string(FILE *stream)
{
	char	buffer[512], *string;
	int		len;

	if (expect(stream, '"') == 0)
		return (NULL); // error
	len = parse_chars(stream, buffer);
	if (len == -1)
		return (NULL); // error
	if (expect(stream, '"') == 0)
		return (NULL); // error	
	string = calloc(len, sizeof(char));
	if (string == NULL)
		return (NULL); // error
	for (int i = 0; buffer[i]; i++)
		string[i] = buffer[i];
	return (string);
}

// '"bonjour"'
// => (json){.type = STRING, .string = "bonjour"};
int	parse_string(json *dst, FILE *stream)
{	
	dst->type = STRING;
	dst->string = allocate_string(stream);
	// if (dst->string == NULL)
		// error
	return (0); // ???
}

// ########################################################################


// $> echo -n '{"tomatoes":42,"potatoes":234}' | ./argo /dev/stdin | cat -e
// $> echo -n '{"recursion":{"recursion":{"recursion":{"recursion":"recursion"}}}}' | ./argo /dev/stdin | cat -e
int	parse_map(json *dst, FILE *stream)
{
	pair	*pairs;
	int		c;
	int		size;

	expect(stream, '{');
	dst->type = MAP;

	c = peek(stream);
	// if (c == EOF)
		// error ? end ?
	size = 0;
	while (c == '"') // supposedy. c != EOF && c == '"'
	{
		if (size == 0)
			pairs = malloc(++size * sizeof(pair));
		else
			pairs = realloc(pairs, ++size * sizeof(pair));
		// if (pairs == NULL)
			// error

		pairs[size - 1].key = allocate_string(stream);
		// if (pairs[size - 1].key == NULL)
			// error

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


// ########################################################################


int	argo(json *dst, FILE *stream)
{
	// What to do?

	// = Just try to write the code for the happy path
	// = Then manage the errors...

	// --------------------------

	// stream (FILE *stream) reads stdin in most examples...

	// Token to token (LL(1))
	// => getc (getc(FILE *stream))

	// --------------------------

	// dst is a pointer to the json structure
	// json is type / (map | integer | string)
	parse_value(dst, stream); // Next, return -1 for failure.
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
