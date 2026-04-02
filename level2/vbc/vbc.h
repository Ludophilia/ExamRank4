/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 21:48:57 by jegerman          #+#    #+#             */
/*   Updated: 2026/04/03 00:13:14 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
// #include <malloc.h> // change this to <stdlib.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

typedef struct node {
	enum {
		ADD,
		MULTI,
		VAL
	}	type;
	int		val;
	struct node *l;
	struct node *r;
}	node;

// TREE
node    *new_node(node n);
void    destroy_tree(node *n);

// ERROR

void    unexpected(char c);
int		expect(char **s, char c);

// PARSING
node    *parse_expr(char *s);

// EVAL
int		eval_tree(node *tree);
