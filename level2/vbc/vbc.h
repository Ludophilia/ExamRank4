/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 21:48:57 by jegerman          #+#    #+#             */
/*   Updated: 2026/07/12 14:52:34 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

typedef enum e_nodetype
{
	ADD,
	MULTI,
	VAL
}	t_nty;

typedef struct node
{
	t_nty			type;
	int				val;
	struct node 	*l;
	struct node 	*r;
}	node;

// TREE
node    *new_node(t_nty type);
void    destroy_tree(node *n);

// ERROR + PARSING

void    unexpected(char c);
int		accept(char **s, char c);
int		expect(char **s, char c);

node    *parse_expr(char **s);
node	*parse_term(char **s);
node	*parse_factor(char **s);
node	*parse_digit(char **s);

// EVAL
int		eval_tree(node *tree);
