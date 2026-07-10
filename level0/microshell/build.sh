#! /bin/bash

cc -Wall -Wextra -Werror -g3 microshell.c pipeline.c utils.c cmds.c -o microshell
# cc microshell.c -o microshell
