/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 20:22:18 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/20 23:24:14 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rewrite_line(t_input_info *terminal, char *line, t_bool erase_mode,
t_bool duplicate_string)
{
	if (erase_mode)
		free(terminal->line);
	if (duplicate_string)
		terminal->line = ft_strdup(terminal->current_history->content);
	else
		terminal->line = line;
	terminal->index = ft_strlen(terminal->line);
	terminal->length = terminal->index;
	ft_putstr_fd("\r\33[0J", STDIN_FILENO);// TODO: maybe: mover el cursor solo hasta index == no tener que reescribir el prompt
	write_prompt();
	ft_putstr_fd(terminal->line, STDIN_FILENO);
}

void	retrieve_older_command(t_input_info *terminal)
{
	if (terminal->current_history)
	{
		if (!terminal->current_history->prev && !terminal->h_saved_line)
		{
			terminal->h_saved_line = terminal->line;
			rewrite_line(
				terminal, terminal->current_history->content, false, true);
		}
		else
		{
			if (terminal->current_history->next)
				terminal->current_history = terminal->current_history->next;
			rewrite_line(
				terminal, terminal->current_history->content, true, true);
		}
	}
}

void	retrieve_newer_command(t_input_info *terminal)
{
	if (terminal->current_history)
	{
		if (!terminal->current_history->prev)
		{
			if (terminal->h_saved_line)
			{
				rewrite_line(terminal, terminal->h_saved_line, false, false);
				terminal->h_saved_line = 0;
			}
		}
		else
		{
			terminal->current_history = terminal->current_history->prev;
			rewrite_line(
				terminal, terminal->current_history->content, true, true);
		}
	}
}

void	handle_input_history(t_input_info *terminal, char direction)
{
	if (direction == 'A')
		retrieve_older_command(terminal);
	else
		retrieve_newer_command(terminal);
}

void	delete_h_saved_line(t_input_info *terminal)
{
	if (terminal->h_saved_line)
	{
		free(terminal->h_saved_line);
		terminal->h_saved_line = 0;
	}
}
