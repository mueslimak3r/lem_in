# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: calamber <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/17 11:20:37 by calamber          #+#    #+#              #
#    Updated: 2019/05/19 22:27:33 by alkozma          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = lem-in

SRC = main.c read.c

OBJ = $(SRC:.c=.o)

SRC_PATH = src/

SRC_POS = $(addprefix $(SRC_PATH),$(SRC))

INC = -I includes

CC = gcc

FLAGS = -Wall -Wextra -Werror -g -fsanitize=address

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft
	@gcc $(FLAGS) $(INC) $(OBJ) libft/libft.a -ltermcap -o $(NAME)

$(OBJ):
	@$(CC) $(FLAGS) -c $(SRC_POS)

clean:
	@make clean -C libft
	@rm -f $(OBJ)

fclean: clean
	@make fclean -C libft
	@rm -f $(NAME)

re: fclean all

again: re
	@make clean

cleanup: fclean
	@rm -rf *ft_ls*

.PHONY : all, re, clean, fclean, again, cleanup
