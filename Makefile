# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yousifgurma <yousifgurma@student.42.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/19 15:52:11 by ygurma            #+#    #+#              #
#    Updated: 2024/02/22 14:37:12 by yousifgurma      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I./includes -I/opt/homebrew/Cellar/readline/8.2.7/include


LIBS =  -L/opt/homebrew/Cellar/readline/8.2.7/lib -lreadline

SRCDIR = sources
BUILTINDIR = builtins
TOKENISATIONDIR = tokenisation
UTILSDIR = utils
SRCFILES = main.c parse_cmd.c execute_cmd.c signal_handler.c redirections.c \
           $(BUILTINDIR)/cd.c $(BUILTINDIR)/echo.c $(BUILTINDIR)/export.c $(BUILTINDIR)/env.c $(BUILTINDIR)/exit.c $(BUILTINDIR)/pwd.c \
           $(TOKENISATIONDIR)/tokenisation.c \
		   $(UTILSDIR)/utils.c $(UTILSDIR)/utils1.c

# Prepend SRCDIR to each source file
SRCS = $(addprefix $(SRCDIR)/, $(SRCFILES))
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(NAME) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
