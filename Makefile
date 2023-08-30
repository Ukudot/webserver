# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/21 11:08:49 by adi-stef          #+#    #+#              #
#    Updated: 2023/08/30 15:56:11 by gpanico          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv
# files
SRC		= main.cpp ServSocket.cpp Utils.cpp Connection.cpp
OBJ		= $(SRC:%.cpp=%.o)

CC		= c++
FLAGS	= -Wall -Wextra -Werror -std=c++98
RM		= rm -f
# colors
RED		= \033[0;31m
GREEN	= \033[0;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
RESET	= \033[0;0m

%.o : %.cpp
	@printf "\r\033[K$(GREEN)Compiling...$(RESET)"
	@$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@printf "\r\033[K$(GREEN)$(NAME) compiled successfully$(RESET)\n"

all: $(NAME)

clean:
	@$(RM) $(OBJ)
	@printf "$(RED)object files removed$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "$(RED)executable removed$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re
