# PROGRAM NAME
NAME = minishell
# HEADER FILES DIRECTORIES
INCLUDES = -Ilibft/ -I.
SRCS =	builtins/builtins.c						\
		builtins/cd.c							\
		builtins/exit.c							\
		builtins/export.c						\
		builtins/unset.c						\
		command_execution/command_checks.c		\
		command_execution/command_io.c			\
		command_execution/execute_commands.c	\
		command_parsing/command_split.c			\
		command_parsing/parsing_insertions.c	\
		command_parsing/parsing_quotations.c	\
		command_parsing/parsing_utils.c			\
		command_parsing/print_parsing_error.c	\
		command_parsing/split_commands.c		\
		handle_input/copy.c						\
		handle_input/handle_input.c				\
		handle_input/handle_keys.c				\
		handle_input/input_history.c			\
		handle_input/paste.c 					\
		handle_input/read_input.c				\
		utils/environment.c						\
		utils/executables_paths.c				\
		utils/handle_error.c					\
		utils/memory_handling.c					\
		utils/minishell_utils.c

LIBFT_A = libft.a
# COMPILER FLAGS
FLAGS = -Wall -Wextra -Werror -g

all: ${NAME}
bonus: all

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