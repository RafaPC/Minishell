# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/21 18:09:12 by rprieto-          #+#    #+#              #
#    Updated: 2021/03/07 10:23:47 by aiglesia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
EXTRA = libft.a
SRCS =	ft_atoi.c					\
		ft_bzero.c					\
		ft_isalnum.c				\
		ft_isalpha.c				\
		ft_isascii.c				\
		ft_isdigit.c				\
		ft_isprint.c				\
		ft_memccpy.c				\
		ft_memchr.c					\
		ft_memcmp.c					\
		ft_memcpy.c					\
		ft_memmove.c				\
		ft_memset.c					\
		ft_strchr.c					\
		ft_strdup.c					\
		ft_strlcat.c				\
		ft_strlcpy.c				\
		ft_strlen.c					\
		ft_strncmp.c				\
		ft_strnstr.c				\
		ft_strrchr.c				\
		ft_tolower.c				\
		ft_toupper.c				\
		ft_putchar_fd.c				\
		ft_putendl_fd.c				\
		ft_putnbr_fd.c				\
		ft_putstr_fd.c				\
		ft_itoa.c					\
		ft_calloc.c					\
		ft_split.c					\
		ft_strjoin.c				\
		ft_strmapi.c				\
		ft_substr.c					\
		ft_strtrim.c				\
		ft_isspace.c				\
		ft_get_index_of.c			\
		ft_checkchar.c				\
		ft_nbrlen.c					\
		ft_abs.c					\
		get_next_line.c				\
		gnl_buffer.c        		\
		ft_insert.c					\
		ft_extract.c				\
		ft_strncat_in.c				\
		ft_array_size.c				\
		ft_alloc.c					\
		ft_buffer_fd.c				\
		ft_array_clear.c			\
		ft_strncpy.c				\
		ft_lstadd_back_bonus.c		\
		ft_lstnew_bonus.c			\
		ft_lstlast_bonus.c			\
		ft_lstadd_front_bonus.c		\
		ft_lstsize_bonus.c			\
		ft_lstdelone_bonus.c		\
		ft_lstiter_bonus.c			\
		ft_lstclear_bonus.c			\
		ft_lstmap_bonus.c			\
		ft_lstmove_backwards.c		\
		ft_str_checkset.c

SRCS_PRINTF = 	ft_printf/ft_hex.c			\
				ft_printf/ft_number.c		\
				ft_printf/ft_printf_utils.c	\
				ft_printf/ft_printf.c		\
				ft_printf/ft_strings.c

OBJS = ${SRCS:.c=.o}
OBJS_PRINTF =	ft_hex.o			\
				ft_number.o			\
				ft_printf_utils.o	\
				ft_printf.o			\
				ft_strings.o
CC = gcc -c -I ./
CFLAGS = -Wall -Werror -Wextra
RM = rm -rf
all: ${NAME}
${NAME}: ${SRCS}
		${CC} ${CFLAGS} ${SRCS} ${SRCS_PRINTF}
		ar rc ${NAME} ${OBJS} ${OBJS_PRINTF}
		ranlib ${EXTRA}
clean:
		${RM} ${OBJS} ${OBJS_PRINTF}
fclean:	clean
		${RM} ${NAME}
re: fclean all
.PHONY:     all re clean fclean