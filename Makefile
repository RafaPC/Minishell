# PROGRAM NAME
NAME = minishell.out
# HEADER FILES DIRECTORIES
INCLUDES = -Ilibft/ -I.
SRCS =	utils/executables_paths.c				\
		utils/handle_error.c					\
		utils/memory_handling.c					\
		utils/minishell_utils.c					\
		builtins/builtins.c						\
		builtins/export.c						\
		builtins/unset.c						\
		command_parsing/command_split.c			\
		command_parsing/parsing_insertions.c	\
		command_parsing/split_commands.c		\
		command_parsing/print_parsing_error.c	\
		command_parsing/read_input.c			\
		temp/execute_commands.c

LIBFT_A = libft.a
# COMPILER FLAGS
FLAGS = -Wall -Wextra -Werror -g

all: ${NAME}

${NAME}: main.c ${SRCS} ${LIBFT_A}
			gcc ${INCLUDES} main.c ${SRCS} ${LIBFT_A} -o ${NAME}

test: test_main.c ${SRCS} ${LIBFT_A}
			gcc ${INCLUDES} ${FLAGS} test_main.c ${SRCS} ${LIBFT_A} -o minishell_test.out

${LIBFT_A}:
		make -C libft
		cp libft/libft.a .
clean:
		rm -f *.o
		rm -f ${NAME}
		rm -f minishell_test.out
		rm -f debug.out
		make -C libft/ clean
fclean: clean
		rm -f libft.a
re:
		make fclean
		make test