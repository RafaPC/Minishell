/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 20:24:45 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/20 23:12:46 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_h_saved_line(t_input_info *terminal)
{
	if (terminal->h_saved_line)
	{
		free(terminal->h_saved_line);
		terminal->h_saved_line = 0;
	}
}

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

void	handle_ctr_c_signal(t_input_info *terminal)
{
	free(terminal->line);
	terminal->line = ft_strdup("");
	terminal->index = 0;
	terminal->length = 0;
	delete_h_saved_line(terminal);
	ft_putchar_fd('\n', STDIN_FILENO);
	write_prompt();
}

t_bool	read_input(t_input_info *terminal)
{
	char	buffer[1];

	while (read(STDIN_FILENO, buffer, 1))
	{
		if (buffer[0] == end_of_transmission && !(terminal->line[0]))
			return (false);
		else if (buffer[0] == ctrl_c)
			handle_ctr_c_signal(terminal);
		else if (buffer[0] == '\n')
			break ;
		else if (buffer[0] == escape)
			handle_keys(terminal);
		else if (buffer[0] == delete)
			delete_char(terminal);
		else if (buffer[0] != '\t')
			add_char(terminal, buffer[0]);
	}
	write(STDIN_FILENO, "\n", 1);
	if (*(terminal->line) && (!terminal->history ||
	ft_strncmp(terminal->line, terminal->history->content, terminal->length)))
		ft_lstdbl_add_front(&terminal->history,
			ft_lstdbl_new(ft_strdup(terminal->line)));
	delete_h_saved_line(terminal);
	return (true);
}
