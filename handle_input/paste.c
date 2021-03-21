/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paste.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 10:00:28 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/21 22:13:23 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
}
