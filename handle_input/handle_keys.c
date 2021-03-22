/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 20:37:04 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/22 11:37:10 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Moves the cursor to the beginning of the current word
** (or previous if the cursor is on a space or already
** at the beginning of a word);
**
** To do so, if at the beginning of a word (and lenght != 1),
** the cursor is moved left once;
** then while space, the cursor is moved to the left until it finds a word;
** Then the cursor is moved left until a new space is found
** or, in both cases, the cursor reaches index 0;
*/

void	move_word_left(t_input_info *terminal)
{
	if (!ft_isspace(terminal->line[terminal->index]) &&
	terminal->index && ft_isspace(terminal->line[terminal->index - 1]))
		move_cursor(terminal, left, true, 1);
	while (terminal->index && ft_isspace(terminal->line[terminal->index]))
		move_cursor(terminal, left, true, 1);
	while (terminal->index && !ft_isspace(terminal->line[terminal->index - 1]))
		move_cursor(terminal, left, true, 1);
}

/*
** Moves the cursor to the end of the current word
** (or next if the cursor is on a space);
**
** To do so, if space, the cursor is moved to the right
** until it finds a word;
** Then the cursor is moved right until a new space is found
** or, in both cases, the cursor reaches string lenght;
*/

void	move_word_right(t_input_info *terminal)
{
	while (terminal->index < terminal->length &&
	ft_isspace(terminal->line[terminal->index]))
		move_cursor(terminal, right, true, 1);
	while (terminal->index < terminal->length &&
	!ft_isspace(terminal->line[terminal->index]))
		move_cursor(terminal, right, true, 1);
}

/*
** Self explanatory, really.
**
** Key codes:
**
** [D = left arrow;
** [C = right arrow;
** [H = home;
** [F = end;
** [1;5D = CTRL + left arrow;
** [1;5C = CTRL + right arrow;
*/

void	handle_movement_keys(t_input_info *terminal, char *buffer)
{
	if (!ft_strncmp(buffer, "[D", 3))
		move_cursor(terminal, left, true, 1);
	else if (!ft_strncmp(buffer, "[C", 3))
		move_cursor(terminal, right, true, 1);
	else if (!ft_strncmp(buffer, "[H", 3))
		move_cursor(terminal, left, true, terminal->index);
	else if (!ft_strncmp(buffer, "[F", 3) &&
	terminal->index != terminal->length)
		move_cursor(terminal, right, true, terminal->length - terminal->index);
	else if (!ft_strncmp(buffer, "[1;5D", 6))
		move_word_left(terminal);
	else if (!ft_strncmp(buffer, "[1;5C", 6))
		move_word_right(terminal);
}

/*
** Hub function.
** A read of six is made to store the key input code.
** Said code is compared to the following codes to call
** the appropiate function.
**
** Key codes:
**
** [A = up arrow;
** [B = down arrow;
** [3~ = supr;
** [1;5A = CTRL + up arrow;
** [1;5B = CTRL + down arrow;
**
** (note: that the ANSI code also includes the escape
** character, which has been previously read to call
** this function);
**
** For the movement codes, see handle_movement_keys;
**
** Supr:
** Cursor is moved to the right, then
** the character is deleted;
*/

void	handle_keys(t_input_info *terminal)
{
	char	buffer[6];

	ft_memset(buffer, 0, 6);
	read(STDIN_FILENO, buffer, 6);
	handle_movement_keys(terminal, (char *)buffer);
	if (!ft_strncmp(buffer, "[A", 3) || !ft_strncmp(buffer, "[B", 3))
		handle_input_history(terminal, buffer[1]);
	else if (!ft_strncmp(buffer, "[3~", 4) &&
	(terminal->index < terminal->length))
	{
		move_cursor(terminal, right, true, 1);
		delete_char(terminal);
	}
	else if (!ft_strncmp(buffer, "[1;5A", 6))
		copy_mode(terminal);
	else if (!ft_strncmp(buffer, "[1;5B", 6))
		paste(terminal);
}
