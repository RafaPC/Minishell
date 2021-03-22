/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 20:22:18 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/22 11:37:54 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Rewrites the line with the provided line, updating terminal->line;
** To do so, the whole line is deleted through the ANSI codes "\r\33[0J":
** 	\r sets the cursor to the beggining of the line;
** 	\33[0J deletes everything from the cursor to the right
** The prompt is then rewriten, as is the new line.
** The index and lenght fields is also updated accordingly;
**
** If set to erase mode, the previous line is freed;
** Similarly, if set to duplicate string, the value saved to terminal->line is a
** duplicate of the provided line;
*/

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
	ft_putstr_fd("\r\33[0J", STDIN_FILENO);
	write_prompt();
	ft_putstr_fd(terminal->line, STDIN_FILENO);
}

/*
** Called when presing the up key;
**
** Changes input line to the one stored in the command struct (older)
** It also updates the cursor position accordingly
**
** If the struct is pointing to the first element of the list
** (when theres no previous element, that is), the current input
** line is saved to h_saved_line, to be restored if needed;
*/

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

/*
** Called when presing the up key;
**
** Changes input line to the one stored in the command struct (newer)
** It also updates the cursor position accordingly
**
** If the struct is pointing to the first element of the list
** (when theres no previous element, that is), the saved line is
** restored instead; note that since the saved string would otherwise
** be deleted, rewrite line is set to not erase nor duplicate
*/

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

/*
** Erases the saved line;
** Note that this gets called whenever the input
** line is changed, be it by adding characters
** (either through add_char or paste), deleting
** them (delete / supr), or pressing enter;
*/

void	delete_h_saved_line(t_input_info *terminal)
{
	if (terminal->h_saved_line)
	{
		free(terminal->h_saved_line);
		terminal->h_saved_line = 0;
	}
}
