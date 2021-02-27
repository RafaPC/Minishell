# PROGRAM NAME
NAME = minishell.out
# HEADER FILES DIRECTORIES
INCLUDES = -Ilibft/ -Ift_printf/ -I.
SRCS =	utils/executables_paths.c				\
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

LIBFTPRINTF_A = libftprintf.a
# COMPILER FLAGS
FLAGS = -Wall -Wextra -Werror

all: ${NAME}

${NAME}: main.c ${SRCS} ${LIBFTPRINTF_A}
			gcc ${INCLUDES} main.c ${SRCS} ${LIBFTPRINTF_A} -o ${NAME}

test: test_main.c ${SRCS} ${LIBFTPRINTF_A}
			gcc ${INCLUDES} test_main.c ${SRCS} ${LIBFTPRINTF_A} -o minishell_test.out

${LIBFTPRINTF_A}:
		make -C ft_printf
		cp ft_printf/libftprintf.a .
clean:
		rm -f *.o
		rm -f ${NAME}
		rm -f minishell_test.out
		rm -f debug.out
		make -C ft_printf/ clean
fclean: clean
		rm -f libftprintf.a
re:
		make fclean
		make test