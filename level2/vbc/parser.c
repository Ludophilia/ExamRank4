/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 22:19:49 by jegerman          #+#    #+#             */
/*   Updated: 2026/04/06 02:01:29 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vbc.h"


// GRAMMAR RULES.
// A set of symbols and production rules, ie, symbols that can be replaced by
// other symbols down to the terminals in the formal language
// The recursive descent parser translate those productions rules to functions
// that closely mirror those grammar rules

// expr   ::= term ( '+' term )*
// term   ::= factor ( '*' factor )*
// factor ::= digit | '(' expr ')'
// digit  ::= '0' | ... | '9' 
 
// TREES FROM IT

/*	(2 + 3)

         ADD(+)
		 /   \
	  VAL(2)  VAL(3)

*/

/*	(3 + 4) * 5

          MUL(*)
          /    \
		ADD(+) VAL(5)
		/   \
	VAL(3)  VAL(4)
	
*/

/* 1+

Error management: After '+' consumed, there's supposed to be something...

How to implement that here? We will see that tomorrow.

*/


// EXAMPLES

// ./vbc '2+3' | cat -e
// 5$
// ./vbc '(3+4)*5' | cat -e
// 35$

// ./vbc '1+' | cat -e
// Unexpected end of input$
// ./vbc '1+2)' | cat -e
// Unexpected token ')'$
// ./vbc '((1+3)*12+(3*(2+6))' | cat -e
// Unexpected token '2'$

/*
What to do??

- Try to implement the recursive descent parser.
	- Take the production rules, convert them into a set of functions

		// expr   ::= term ( '+' term )*
		// term   ::= factor ( '*' factor )*
		// factor ::= digit | '(' expr ')'
		// digit  ::= '0' | ... | '9' 

*/



/* Use that example to see if

(2 + 3)

         ADD(+)
		 /   \
	  VAL(2)  VAL(3)

2 + 3 + 5

            ADD(+)
           /      \
         ADD(+)  VAL(5)
		 /   \
	  VAL(2)  VAL(3)

*/

// Error management later....
node    *parse_expr(char *s)
{
	node	*node, *new;
	
    // if (*s == 0) // Unsure... Was: if (*s)
    // {
    //     destroy_tree(node);
    //     return (NULL);
    // }

	node = parse_term(s); // ???
	// if (node == NULL) // ???
	// 	return (NULL); // destroy_tree(node), 
	// supposing that pointer would have moved

	while (*s == '+')
	{
		// error management
		accept(&s, '+'); // ???

		new = new_node(ADD);
		// if (new == NULL);
		// 	return (NULL);

		new->l = node;
		new->r = parse_term(s);

		node = new;
	}
	
    return (node);
}

node	*parse_term(char *s)
{
	node *node, *new;

	node = parse_factor(s);
	// if (node == NULL) // ???
	// 	return (NULL);
	
	// ???
	while (*s == '*')
	{
		// error management
		accept(&s, '*'); // ???

		new = new_node(MULTI);
		// if (new == NULL);
		// 	return (NULL);

		new->l = node;
		new->r = parse_term(s);

		node = new;
	}
	return (node);
}

node	*parse_factor(char *s)
{
	node	*node;

	// parse_digit
	if (isdigit(*s)) // Error management?
	{
		node = new_node(VAL);
		if (node == NULL)
			return (NULL); // ???
		node->val = (*s - '0');
		accept(&s, *s); // Move to the next token???
		// return (node);
	}
	// parse_expr
	if (*s == '(') // Really?
	{
		accept(&s, '('); // ???
		node = parse_expr(s);
		if (node == NULL)
			return (NULL); // ???
		expect(&s, ')');
	}

	// ???
	
	return (node);
}


int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    node *tree = parse_expr(argv[1]);
    if (!tree)
        return (1);

    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);

	return (0);
}
