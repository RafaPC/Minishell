# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/04/07 17:19:14 by rprieto-          #+#    #+#              #
#    Updated: 2021/02/22 19:13:57 by rprieto-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = libftprintf.a
LIBFT_A = 	libft.a
LIB_DIR = ../libft/
CC = gcc -c 
SRCS = ft_printf.c ft_printf_utils.c ft_strings.c ft_number.c ft_hex.c
OBJS = ${SRCS:.c=.o}

CFLAGS = -Wall -Wextra -Werror

all: ${NAME}

${NAME}: ${OBJS} ${LIB_DIR}${LIBFT_A}
				cp ${LIB_DIR}${LIBFT_A} ${NAME}
				ar rc ${NAME} ${OBJS}
				ranlib ${NAME}

${LIB_DIR}${LIBFT_A}:
				make -C ${LIB_DIR}

${OBJS}: ${SRCS}
				${CC} -I../libft/ ${CFLAGS} ${SRCS}

clean:
		rm -f ${OBJS}
		make -C $(LIB_DIR) clean
		
fclean: clean
		rm -f ${NAME}
		make -C $(LIB_DIR) fclean
		
re: fclean all

.PHONY:	all clean fclean re
