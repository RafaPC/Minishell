/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 20:24:45 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/22 11:39:55 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Holy grail of the read input; Like, really...
**
** Functionality changes based on input;
**
** Moves the cursor to the left or right nb times,
** based on the value of direction.
** When moving to the right, the cursor will move up to terminal->lenght;
** Similarly, when moving to the left the
** cursor will only move up to index = 0;
**
** This avoids the cursor moving freely through the line,
** instead being limited to the actual input information.
**
** Additionally, if change index is set to true,
** the index value will not be changed;
*/

void	move_cursor(t_input_info *terminal, int direction, t_bool change_index,
unsigned nb)
{
	if (nb == 0)
		return ;
	if (direction == left && terminal->index)
	{
		if (nb > terminal->index)
			nb = terminal->index;
		if (change_index)
			terminal->index -= nb;
		ft_printf(STDIN_FILENO, "\33[%iD", nb);
	}
	else if (direction == right && terminal->index < terminal->length)
	{
		if (nb > terminal->length - terminal->index)
			nb = terminal->length - terminal->index;
		if (change_index)
			terminal->index += nb;
		ft_printf(STDIN_FILENO, "\33[%iC", nb);
	}
}

/*
** Removes a character to the input line (through ft_insert),
** modifiying lenght and index accordingly.
**
** The console display is also updated by erasing from the current
** position to the right.
**
** Note that if the index is smaller than the lenght, the
** terminal line needs to be updated after the current position
** is erased (through ft_putstr_fd).
** In this case, the cursor position needs to be updated,
** which is done inside the if;
*/

void	delete_char(t_input_info *terminal)
{
	int index;

	if (terminal->index)
	{
		ft_putstr_fd("\b\33[0K", STDIN_FILENO);
		(terminal->index)--;
		(terminal->length)--;
		ft_extract(&terminal->line, terminal->index, 1);
		if (terminal->index != terminal->length)
		{
			ft_putstr_fd(&terminal->line[terminal->index], STDIN_FILENO);
			index = terminal->index;
			terminal->index = terminal->length;
			move_cursor(terminal, left, true, terminal->length - index);
		}
	}
	delete_h_saved_line(terminal);
}

/*
** Adds a character to the input line (through ft_insert),
** modifiying lenght and index accordingly.
**
** The console display is also updated;
** Note that if the index is smaller than the lenght, the
** terminal line needs to be updated after the input char
** is written (through ft_putstr_fd).
** In this case, the cursor position needs to be updated,
** which is done inside the if;
*/

void	add_char(t_input_info *terminal, char c)
{
	char	buffer[2];
	int		index;

	buffer[0] = c;
	buffer[1] = '\0';
	ft_insert(&terminal->line, buffer, terminal->index, 1);
	(terminal->index)++;
	(terminal->length)++;
	ft_putstr_fd(&terminal->line[terminal->index - 1], STDIN_FILENO);
	if (terminal->index != terminal->length)
	{
		index = terminal->index;
		terminal->index = terminal->length;
		move_cursor(terminal, left, true, terminal->length - index);
	}
	delete_h_saved_line(terminal);
}

void	check_input_character(t_input_info *terminal, char c)
{
	if (c == ctrl_c)
		handle_ctr_c_signal(terminal);
	else if (c == escape)
		handle_keys(terminal);
	else if (c == delete)
		delete_char(terminal);
	else if (c != '\t')
		add_char(terminal, c);
}

/*
** Processes input until stopped; this can happen either
** through the ctrl + D signal (on an empty line) or by
** pressing enter.
**
** On the first case, false is returned, which ends up ending
** the program. On enter, however, the input line is saved to
** the history struct, and true is returned, so the input string
** may be processed;
*/

t_bool	read_input(t_input_info *terminal)
{
	char	buffer[1];

	while (read(STDIN_FILENO, buffer, 1))
	{
		if (buffer[0] == end_of_transmission)
		{
			if (!terminal->line[0])
				return (false);
		}
		else if (buffer[0] == '\n')
			break ;
		check_input_character(terminal, buffer[0]);
	}
	write(STDIN_FILENO, "\n", 1);
	if (*(terminal->line) && (!terminal->history ||
	ft_strncmp(terminal->line, terminal->history->content, terminal->length)))
		ft_lstdbl_add_front(&terminal->history,
			ft_lstdbl_new(ft_strdup(terminal->line)));
	delete_h_saved_line(terminal);
	return (true);
}
