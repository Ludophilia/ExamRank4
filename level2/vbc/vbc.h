/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 21:48:57 by jegerman          #+#    #+#             */
/*   Updated: 2026/04/05 21:58:38 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
// #include <malloc.h> // change this to <stdlib.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

enum e_type
{
	ADD,
	MULTI,
	VAL
};

typedef struct node
{
	enum e_type		type;
	int				val;
	struct node 	*l;
	struct node 	*r;
}	node;

// TREE
node    *new_node(enum e_type type);
void    destroy_tree(node *n);

// ERROR

void    unexpected(char c);
int		accept(char **s, char c);
int		expect(char **s, char c);

// PARSING
node    *parse_expr(char *s);

// EVAL
int		eval_tree(node *tree);
