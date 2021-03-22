/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paste.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 10:00:28 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/22 11:37:21 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Pastes the contents of copy_line, if they exist.
** Input line is updated through ft_insert,
** are is the index and lenght values.
**
** The terminal is rewritten, by deleting everything
** from the cursor position rightward. The copy_line
** is then written, as is the remaining of the original
** string;
**
** Finally, the index is set to the lenght value
** (which is the position of the screen cursor)
** and both are set to the position following the
** insertion through the use of move_cursor set to
** update index = true;
*/

void	paste(t_input_info *terminal)
{
	int	aux;
	int	str_lenght;

	if (!terminal->copy_line)
		return ;
	str_lenght = ft_strlen(terminal->copy_line);
	aux = ft_insert(&terminal->line, terminal->copy_line, terminal->index,
	str_lenght);
	ft_putstr_fd("\33[0K", STDIN_FILENO);
	ft_putstr_fd(terminal->copy_line, STDIN_FILENO);
	ft_putstr_fd(&terminal->line[aux], STDIN_FILENO);
	terminal->length += str_lenght;
	terminal->index = terminal->length;
	move_cursor(terminal, left, true, terminal->length - aux);
	delete_h_saved_line(terminal);
}
