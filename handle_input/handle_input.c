/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 23:05:49 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/21 22:13:14 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			write_prompt(void)
{
	char *str;

	str = "Minishell-> ";
	ft_putstr_fd("\033[32m", STDIN_FILENO);
	ft_putstr_fd(str, STDIN_FILENO);
	ft_putstr_fd("\033[0m", STDIN_FILENO);
	return (ft_strlen(str));
}

static int	set_terminal_mode(struct termios *term_save, const int canonical)
{
	char				*term_name;
	struct termios		term;

	if (canonical == 0)
	{
		if ((term_name = getenv("TERM")) == NULL)
			return (-1);
		tcgetattr(STDIN_FILENO, &term);
		tcgetattr(STDIN_FILENO, term_save);
		term.c_lflag &= ~(ICANON | ECHO | ISIG);
		term.c_cc[VMIN] = 1;
		term.c_cc[VTIME] = 0;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
	if (canonical == 1)
		tcsetattr(STDIN_FILENO, TCSANOW, term_save);
	return (0);
}

t_bool		handle_input(char **buffer, t_list_dbl **command_history,
char **copy_line)
{
	t_input_info terminal;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	ft_memset(&terminal, 0, sizeof(t_input_info));
	terminal.line = ft_strdup("");
	terminal.history = *command_history;
	terminal.current_history = terminal.history;
	terminal.copy_line = *copy_line;
	set_terminal_mode(&terminal.term_cp, 0);
	write_prompt();
	if (!read_input(&terminal))
	{
		free(terminal.line);
		set_terminal_mode(&terminal.term_cp, 1);
		return (false);
	}
	set_terminal_mode(&terminal.term_cp, 1);
	*command_history = terminal.history;
	*buffer = terminal.line;
	*copy_line = terminal.copy_line;
	return (true);
}

void		handle_ctr_c_signal(t_input_info *terminal)
{
	free(terminal->line);
	terminal->line = ft_strdup("");
	terminal->index = 0;
	terminal->length = 0;
	delete_h_saved_line(terminal);
	ft_putstr_fd("^C\n", STDIN_FILENO);
	write_prompt();
}
