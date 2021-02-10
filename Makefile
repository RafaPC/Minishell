# PROGRAM NAME
NAME = minishell
# HEADER FILES DIRECTORIES
INCLUDES = -Ilibft/ -I.
SRCS =	main.c									\
		executables_paths.c						\
		read_input.c							\
		minishell_utils.c						\
		builtins/builtins.c						\
		builtins/export.c						\
		builtins/unset.c						\
		command_parsing/command_split.c			\
		command_parsing/parsing_insertions.c	\
		command_parsing/split_commands.c
LIBFT_A = libft.a
# COMPILER FLAGS
FLAGS = -Wall -Wextra -Werror

all: ${NAME}

${NAME}: ${SRCS} ${LIBFT_A}
			gcc ${INCLUDES} ${SRCS} ${LIBFT_A} -o ${NAME}

${LIBFT_A}:
		make bonus -C libft/
		cp libft/libft.a .

clean:
		rm -f *.o
		rm -f ${NAME}
re:
		make clean
		make