/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 20:29:21 by jegerman          #+#    #+#             */
/*   Updated: 2026/04/16 02:01:17 by jegerman         ###   ########.fr       */
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

int parse_value(json *dst, FILE *stream)
{
	// That's just the outline. Nothing works...
	int c = peek(stream);
	// if (c == EOF)
		// error ? end ?
	
	if (isdigit(c))
	{
		parse_integer(dst, stream);
	}
	else if (c == '"')
	{
		parse_string(dst, stream);
	}
	// else if (c == '{')
	// {
	// 	parse_map(dst, stream);
	// }
	return (0);// ??? 
}

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
		// error
		nbr = 10 * nbr + (c - '0');
		c = peek(stream);
		// if (c == EOF)
			// error ? end ?
	}
	dst->integer = nbr;
	return (0); // ???
}

// How do you know if a JSON should be allocated or not?
// We start from a json allocated on stack, or at least something we don't
// control...

// '"bonjour"'
// => (json){.type = STRING, .string = "bonjour"};
int	parse_string(json *dst, FILE *stream)
{
	int		i;
	char	buffer[512], c, d, *string;
	
	expect(stream, '"');
	dst->type = STRING;

	i = 0;
	// Copy to the buffer.
	c = peek(stream);
	// if (c == EOF)
		// error ? end ?
	while (c != EOF && c != '"')
	{
		// IS that enough?
			printf("c -> '%c'\n", c);

		if (c == '\\')
		{
			d = peek(stream);
			// if (d == EOF)
				// error ? end ?
			if (d != '\\' && d != '"') 	// Only \ and ""
				unexpected(stream);

			// ./build.sh && echo -n '"\" "' | ./argo /dev/stdin
			// NOT THERE YET...
			buffer[i++] = c;
			consume(stream);
			c = peek(stream);
			// if (c == EOF)
			// error ? end ?
			printf("c -> '%c'\n", c);
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
	expect(stream, '"'); // Here?

	// Don't forget the /0
	string = calloc(i + 1, sizeof(char));
	// if (string == NULL)
	// 	return (-1); // ???
	for (int j = 0; buffer[j]; j++)
		string[j] = buffer[j];
	
	dst->string = string;
	return (0); // ???
}

// int	parse_map(json *dst, FILE *stream)




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
