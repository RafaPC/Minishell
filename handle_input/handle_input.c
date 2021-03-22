/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 23:05:49 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/22 11:38:13 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Self explanatory, really;
** Of note is that "\033[32m" is the ANSI code to colour the text green;
**
** On the other hand, "\033[0m" resets the colour to white;
*/

int			write_prompt(void)
{
	char *str;

	str = "Minishell-> ";
	ft_putstr_fd("\033[32m", STDIN_FILENO);
	ft_putstr_fd(str, STDIN_FILENO);
	ft_putstr_fd("\033[0m", STDIN_FILENO);
	return (ft_strlen(str));
}

/*
** Functionality varies based on the canonical field;
**
** If false, the terminal is set to non canonical mode,
** with the following implications:
**
** ICANON: The read function will now output read bytes
** instantly, instead of waiting for a page jump or the EOF
**
** ECHO: input character will NOT output to the console.
**
** ISIG: Signals will be IGNORED;
**
** Additionally, a copy of the terminal settings prior to
** the change are stored in term_save.
**
** If on false mode, terminal settings are restored based on
** the copy made in term save;
*/

static int	set_terminal_mode(struct termios *term_save, t_bool canonical)
{
	char				*term_name;
	struct termios		term;

	if (canonical == false)
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
	if (canonical == true)
		tcsetattr(STDIN_FILENO, TCSANOW, term_save);
	return (0);
}

/*
** Sets terminal mode to non chanonical (see set terminal mode)
** and reads input until said function returns;
** If it returns a false, the ctrl + D signal has been triggered
** and the function returns accordingly.
** Alternatively, working info, such as command history or copy line
** is saved to the main structure through pointers.
**
** Either way the terminal is reset to its previous status
*/

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
	set_terminal_mode(&terminal.term_cp, false);
	write_prompt();
	if (!read_input(&terminal))
	{
		free(terminal.line);
		set_terminal_mode(&terminal.term_cp, true);
		return (false);
	}
	set_terminal_mode(&terminal.term_cp, true);
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
