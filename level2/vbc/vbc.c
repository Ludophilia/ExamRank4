/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 22:19:49 by jegerman          #+#    #+#             */
/*   Updated: 2026/07/12 15:03:27 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vbc.h"

node    *parse_expr(char **s)
{
	node	*node, *new;

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
	node	*node, *new;

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
		node = parse_digit(s);
		if (node == NULL)
			return (NULL);
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

node	*parse_digit(char **s)
{
	node	*node;

	node = new_node(VAL);
	if (node == NULL)
		return (NULL);
	node->val = (**s - '0');
	accept(s, **s);
	return (node);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	node *tree = parse_expr(argv + 1);
	if (!tree)
		return (1);
	if (expect(argv + 1, 0) == 0)
		return (destroy_tree(tree), 1);
	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
	return (0);
}
