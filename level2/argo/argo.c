/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 20:29:21 by jegerman          #+#    #+#             */
/*   Updated: 2026/04/11 22:04:42 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argo.h"

int	argo(json *dst, FILE *stream);

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
