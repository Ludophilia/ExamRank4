/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 20:30:12 by jegerman          #+#    #+#             */
/*   Updated: 2026/04/16 01:26:10 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct	json {
	enum {
		MAP,
		INTEGER,
		STRING
	} type;
	union {
		struct {
			struct pair	*data;
			size_t		size;
		} map;
		int		integer;
		char	*string;
	};
}	json;

typedef struct	pair {
	char	*key;
	json	value;
}	pair;



int		consume(FILE *stream);
int		peek(FILE *stream);
void	unexpected(FILE *stream);
int		accept(FILE *stream, char c);
int		expect(FILE *stream, char c);

void	free_json(json j);

void	serialize(json j);

int 	parse_value(json *dst, FILE *stream);
int		parse_integer(json *dst, FILE *stream);
int		parse_string(json *dst, FILE *stream);

int		argo(json *dst, FILE *stream);
