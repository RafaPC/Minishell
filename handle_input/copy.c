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

void	copy_line(t_input_info *terminal, int start_pos)
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
