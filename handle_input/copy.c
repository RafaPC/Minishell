/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 10:00:28 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/21 12:34:34 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rewrite_copy_line(t_input_info *terminal, unsigned current_pos, unsigned start_pos, t_bool background)
{
	int start;
	int end;

	if (background) //Sets writing to write in black with a white background;
	{
		ft_putstr_fd("\033[47m", STDIN_FILENO);
		ft_putstr_fd("\033[30m", STDIN_FILENO);
	}
	ft_putstr_fd("\33[0K", STDIN_FILENO); //Erases from the cursor to the right
	start = current_pos <= start_pos ? current_pos : start_pos; 
	end = current_pos <= start_pos ? start_pos : current_pos;
	while (start <= end)
	{
		ft_putchar_fd(terminal->line[start], STDIN_FILENO); //writes
		start++;
	}
	ft_putstr_fd("\033[0m", STDIN_FILENO); //Reset writing to regular white
	ft_putstr_fd(&terminal->line[start], STDIN_FILENO); //write the rest of the string
	terminal->index = terminal->length;
	move_cursor(terminal, left, true, terminal->length - current_pos); //Moves cursor back to the appropiate position
}


int move_copy_cursor(t_input_info *terminal, unsigned current_pos, unsigned start_pos, unsigned direction)
{
	if (start_pos != current_pos || !direction)
	{
		if (direction == left && current_pos)
		{
			current_pos--;
			rewrite_copy_line(terminal, current_pos, start_pos, true);
		}
		else if (current_pos < terminal->length - 1)
		{
			current_pos++;
			rewrite_copy_line(terminal, current_pos, start_pos, true);
		}
	}
	if (terminal->copy_line)
		free(terminal->copy_line);
	if (start_pos > current_pos)
		terminal->copy_line = ft_strncpy(&terminal->line[current_pos],
		start_pos - current_pos + 1);
	else
		terminal->copy_line = ft_strncpy(&terminal->line[start_pos],
		current_pos - start_pos + 1);
	return (current_pos);
}


void	copy_mode(t_input_info *terminal)
{
	unsigned	start_pos;
	unsigned	current_pos;
	char 		buffer[6];

	ft_memset(buffer, 0, 4); //If index == length move cursor left?
	if (terminal->index == terminal->length)
		move_cursor(terminal, left, true, 1);
	start_pos = terminal->index;
	current_pos = terminal->index;
	move_copy_cursor(terminal, current_pos, start_pos, 0);
	rewrite_copy_line(terminal, current_pos, start_pos, true);

 	while (true)
 	{
		ft_memset(buffer, 0, 6);
 		read(STDIN_FILENO, buffer, 6);
 		if (buffer[0] == escape)
 		{
 			if (!ft_strncmp(buffer, "[1;5A", 6))
 			{
 				rewrite_copy_line(terminal, current_pos, start_pos, false);
 				return ;
 			}
 			if (!ft_strncmp(buffer, "[D", 3))
 				current_pos = move_copy_cursor(terminal, current_pos, start_pos, left);
 			else if (!ft_strncmp(buffer, "[C", 3))
 				current_pos = move_copy_cursor(terminal, current_pos, start_pos, right);
 		}
 	}
}