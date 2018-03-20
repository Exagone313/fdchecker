# Copyright (C) 2018, Elouan Martinet <exa@elou.world>
# All rights reserved.
#
# This file is part of fdchecker.
#
# fdchecker is free software. It comes without any warranty, to
# the extent permitted by applicable law. You can redistribute it
# and/or modify it under the terms of the Do What the Fuck You Want
# to Public License, Version 2, as published by Sam Hocevar. See
# <http://www.wtfpl.net/> for more details.

NAME ?= fdchecker

CFLAGS += -Wall -Wextra -Werror

SRC := \
	fdchecker.c

OBJ := $(SRC:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(OBJ)

fclean:
	$(MAKE) clean
	$(RM) $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all
