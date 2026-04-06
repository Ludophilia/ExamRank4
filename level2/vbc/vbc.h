/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 21:48:57 by jegerman          #+#    #+#             */
/*   Updated: 2026/04/06 18:40:39 by jegerman         ###   ########.fr       */
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
}	t_noty;

typedef struct node
{
	t_noty			type;
	int				val;
	struct node 	*l;
	struct node 	*r;
}	node;

// TREE
node    *new_node(t_noty type);
void    destroy_tree(node *n);

// ERROR

void    unexpected(char c);
int		accept(char **s, char c);
int		expect(char **s, char c);

// PARSING
node    *parse_expr(char **s);
node	*parse_term(char **s);
node	*parse_factor(char **s);

// EVAL
int		eval_tree(node *tree);
