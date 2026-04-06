/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 22:19:49 by jegerman          #+#    #+#             */
/*   Updated: 2026/04/07 01:37:46 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vbc.h"

/* GRAMMAR RULES.
A set of symbols and production rules, ie, symbols that can be replaced by
other symbols down to the terminals in the formal language
The recursive descent parser translate those productions rules to functions
that closely mirror those grammar rules below.

expr   ::= term ( '+' term )*
term   ::= factor ( '*' factor )*
factor ::= digit | '(' expr ')'
digit  ::= '0' | ... | '9' 

1 + 1

     ADD(+)
	 /   \
  VAL(1)  VAL(1)

(2 + 3)

     ADD(+)
	 /   \
  VAL(2)  VAL(3)

(3 + 4) * 5

      MUL(*)
      /    \
	ADD(+) VAL(5)
	/   \
VAL(3)  VAL(4)

2 + 3 + 5

    ADD(+)
   /      \
 ADD(+)  VAL(5)
 /   \
VAL(2)  VAL(3)

*/

/*

Error management: After '+' consumed, there's supposed to be something...

What could possibly go wrong?

- [o] Empty input: ./vbc ""

- [o] Missing operand with + or *: ./vbc "+3"; ./vbc "2*".
	- [ ] Expected token after '+' or '*': digit 

- [o] Missing opening, closing parentheses: ./vbc "4("; ./vbc "3)"
	- [ ] Expected token after '(': '(', digit 
	- [ ] Expected token after ')': ')', null, '+', '*' 

- [o] Wrong starting character
	- [ ] Expected token at start: digit or '('

- [o] Wrong ending character
	- [ ] Expected token at end: null
	
- [o] allocation errors
*/ 

node    *parse_expr(char **s)
{
	node	*node, *new;

	// if (*s == 0) // Unsure... Was: if (*s)
	// {
	//     destroy_tree(node);
	//     return (NULL);
	// }
	node = parse_term(s);
	if (node == NULL)
		return (NULL);
	while (**s == '+')
	{
		accept(s, '+');
		new = new_node(ADD);
		if (new == NULL)
			return (destroy_tree(node), NULL);
		new->l = node;
		new->r = parse_term(s);
		if (new->r == NULL)
			return (destroy_tree(new), NULL);
		node = new;
	}
    return (node);
}

node	*parse_term(char **s)
{
	node *node, *new;

	node = parse_factor(s);
	if (node == NULL)
		return (NULL);
	while (**s == '*')
	{
		accept(s, '*');
		new = new_node(MULTI);
		if (new == NULL)
			return (destroy_tree(node), NULL);
		new->l = node;
		new->r = parse_factor(s);
		if (new->r == NULL)
			return (destroy_tree(new), NULL);
		node = new;
	}
	return (node);
}

node	*parse_factor(char **s)
{
	node	*node;

	if (isdigit(**s))
	{
		node = new_node(VAL);
		if (node == NULL)
			return (NULL);
		node->val = (**s - '0');
		accept(s, **s);
		return (node);
	}
	if (**s == '(')
	{
		accept(s, '(');
		node = parse_expr(s);
		if (node == NULL)
			return (NULL);
		if (expect(s, ')') == 0)
			return (destroy_tree(node), NULL);
		return (node);
	}
	unexpected(**s);
	return (NULL);
}


int main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	// printf("argc -> %i\n", argc);
	// printf("[MAIN] argv[1] -> %s\n", argv[1]);
	node *tree = parse_expr(argv + 1);
	if (!tree)
		return (1);
	// printf("[MAIN] argv[1] -> '%s'\n", argv[1]);
	if (expect(argv + 1, 0) == 0)
		return (destroy_tree(tree), 1);
	//....
	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
	return (0);
}
