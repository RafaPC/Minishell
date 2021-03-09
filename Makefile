# PROGRAM NAME
NAME = minishell
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
		command_parsing/parsing_checks.c		\
		command_parsing/print_parsing_error.c	\
		command_parsing/read_input.c			\
		temp/debug_minishell.c					\
		temp/execute_commands.c

LIBFT_A = libft.a
# COMPILER FLAGS
FLAGS = -Wall -Wextra -Werror -g

all: ${NAME}

${NAME}: main.c ${SRCS} ${LIBFT_A}
			gcc ${INCLUDES} ${FLAGS} main.c ${SRCS} ${LIBFT_A} -o ${NAME}

${LIBFT_A}:
		make -C libft
		cp libft/libft.a .
clean:
		make -C libft clean
		rm -f *.o
fclean: clean
		make -C libft fclean
		rm -f libft.a
		rm -f ${NAME}
		rm -f debug.out
re: fclean all