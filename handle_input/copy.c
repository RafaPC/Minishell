/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 10:00:28 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/22 11:35:32 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Rewrites the terminal line from the range
** given by current pos and start pos;
**
** To do so, it calculates the appropiate start
** point and erases the line rightwards.
** The terminal is then set to write in black with
** a white colour, if background mode is activated.
** The given range is printed in the given mode;
** Terminal writing mode is rest through "\033[0m"
** and the remainder of the string (if any) is written
** in regular text;
**
** Finally, the cursor position is updated so it points
** at the cursor position variable;
*/

void	rewrite_copy_line(t_input_info *terminal, unsigned current_pos,
	unsigned start_pos, t_bool background)
{
	unsigned int	start;
	unsigned int	end;

	if (terminal->index == terminal->length)
		return ;
	start = current_pos <= start_pos ? current_pos : start_pos;
	end = current_pos <= start_pos ? start_pos : current_pos;
	if (start < terminal->index)
		move_cursor(terminal, left, true, terminal->index - start);
	ft_putstr_fd("\33[0K", STDIN_FILENO);
	if (background)
	{
		ft_putstr_fd("\033[47m", STDIN_FILENO);
		ft_putstr_fd("\033[30m", STDIN_FILENO);
	}
	while (start <= end)
	{
		ft_putchar_fd(terminal->line[start], STDIN_FILENO);
		start++;
	}
	ft_putstr_fd("\033[0m", STDIN_FILENO);
	ft_putstr_fd(&terminal->line[start], STDIN_FILENO);
	terminal->index = terminal->length;
	move_cursor(terminal, left, true, terminal->length - current_pos);
}

/*
** Copies the line from start to end position;
*/

void	copy_line(t_input_info *terminal, unsigned int start_pos)
{
	if (terminal->index == start_pos)
		terminal->copy_line = ft_strncpy(&terminal->line[terminal->index], 1);
	else if (terminal->index < start_pos)
		terminal->copy_line = ft_strncpy(&terminal->line[terminal->index],
				start_pos - terminal->index + 1);
	else
		terminal->copy_line = ft_strncpy(&terminal->line[start_pos],
				terminal->index - start_pos + 1);
}

/*
** Moves cursor based on the input found in buffer;
** If the cursor has been moved so that index = teminal lenght
** the cursor is moved left once so it points to string input;
**
** The terminal line is then rewritten so the current selection has
** a white background;
**
** The position of the index is saved prior to the movement.
**
** If the movement has been to the right (and thus aux < index)
** white bacground needs to be removed in the aux-index range;
** Therefore, the line is rewritten, (storing the final value of index in aux)
** using rewrite_copy_line from 0 to index, set to FALSE.
** Then the index is restored through move_cursor;
**
** Either way, rewrite copy line is called from index to start position,
** so that said range has a white background;
**
** Finally, the copy line is freed and the new one is stored;
*/

void	copy_logic(t_input_info *terminal, char *buffer, int start_pos)
{
	unsigned int	aux;

	aux = terminal->index;
	handle_movement_keys(terminal, (char *)buffer);
	if (terminal->index == terminal->length)
		move_cursor(terminal, left, true, 1);
	if (aux < terminal->index)
	{
		aux = terminal->index;
		rewrite_copy_line(terminal, 0, terminal->index, false);
		move_cursor(terminal, right, true, aux);
	}
	rewrite_copy_line(terminal, terminal->index, start_pos, true);
	free(terminal->copy_line);
	copy_line(terminal, start_pos);
}

/*
** Places the program in "copy mode".
** In this mode, all input is ommited, safe for movement input
** the ctrl + c signal and ctrl + up key:
**
** Ctrl + c interrupts copy mode and handles ctrl + c signal normally;
**
** Movement input updates the position of the cursor regularly;
**
** ctrl + up: ends copy mode;
**
** After the position has been changed, a new copy line is generated.
** (see copy logic);
*/

void	handle_copy_movement(t_input_info *terminal, unsigned int start_pos)
{
	char			buffer[6];

	ft_memset(buffer, 0, 6);
	while (true)
	{
		buffer[0] = '\0';
		read(STDIN_FILENO, buffer, 1);
		if (buffer[0] == escape)
		{
			read(STDIN_FILENO, buffer, 6);
			if (!ft_strncmp(buffer, "[1;5A", 6))
			{
				rewrite_copy_line(terminal, terminal->index, start_pos, false);
				return ;
			}
			copy_logic(terminal, (char *)buffer, start_pos);
			ft_memset(buffer, 0, 6);
		}
		else if (buffer[0] == ctrl_c)
		{
			handle_ctr_c_signal(terminal);
			return ;
		}
	}
}

/*
** Copy functionality super function.
** If there's no input, signified by index = 0, nothing is done;
** If index = length, the index is moved left once so it points to actual
** content of the input string;
** A copy string of size one is made, copying the current selected char
** (whose background is set to white to signify copy mode);
** The program is then put in "copy mode" through rewrite copy line, where
** it will get the string to copy;
** Note that the current index position is set as the start position;
*/

void	copy_mode(t_input_info *terminal)
{
	if (terminal->index == terminal->length)
	{
		if (!terminal->index)
			return ;
		move_cursor(terminal, left, true, 1);
	}
	rewrite_copy_line(terminal, terminal->index, terminal->index, true);
	if (terminal->copy_line)
		free(terminal->copy_line);
	terminal->copy_line = ft_strncpy(&terminal->line[terminal->index], 1);
	handle_copy_movement(terminal, terminal->index);
}
