/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 22:19:49 by jegerman          #+#    #+#             */
/*   Updated: 2026/04/03 01:48:15 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vbc.h"

int	vbc(char *expr)
{
	
}

// GRAMMAR RULES.
// A set of symbols and production rules, ie, symbols that can be replaced by
// other symbols down to the terminals in the formal language
// The recursive descent parser translate those productions rules to functions
// that closely mirror those grammar rules

// expr   ::= term ( '+' term )*
// term   ::= factor ( '*' factor )*
// factor ::= digit | '(' expr ')'

// TREES FROM IT

/*	(2 + 3)

         ADD(+)
		 /   \
	  VAL(2)  VAL(3)

*/

/*	(3+4)*5

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
