/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 20:29:21 by jegerman          #+#    #+#             */
/*   Updated: 2026/04/15 02:46:17 by jegerman         ###   ########.fr       */
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

	// if c == EOF... (error or end)
	
	if (isdigit(c))
	{
		parse_integer(dst, stream);
		// no expect?
	}
	else if (c == '"')
	{
		parse_string(dst, stream);
		// -> expect '"' (later)
	}
	else if (c == '{')
	{
		parse_map(dst, stream);
		// -> expect '}' (later)
	}
	return (0);// ??? 
}

// '"bonjour"'
// => (json){.type = STRING, .string = "bonjour"};
int	parse_string(json *dst, FILE *stream)
{
	// How do you know if a JSON should be allocated or not?
	// We start from a json allocated on stack, or at least something we don't
	// control...
	int	len;
	// char	str[]
	
	dst->type = STRING;
	// fscanf(stream, "%d", &dst->integer);

	return (0); // ???
}

// int	parse_map(json *dst, FILE *stream)


// '1'
// => (json){.type = INTEGER, .integer = 1};
int	parse_integer(json *dst, FILE *stream)
{
	// How do you know a JSON should be allocated or not?
	// We start from a json allocated on stack, or at least something we don't
	// control...
	dst->type = INTEGER;
	// Does this move the filepos? It seems like it...
	fscanf(stream, "%d", &dst->integer);
	return (0); // ???

}

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

	parse_value(dst, stream); // ???

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
