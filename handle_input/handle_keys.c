/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 20:37:04 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/18 20:48:45 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void move_word_left(t_input_info *terminal)
{
    if(!ft_isspace(terminal->line[terminal->index]) &&
    terminal->index && ft_isspace(terminal->line[terminal->index]))
        move_cursor(terminal, left, true, 1);
    while (terminal->index && ft_isspace(terminal->line[terminal->index]))
        move_cursor(terminal, left, true, 1);
    while (terminal->index && ft_isspace(terminal->line[terminal->index - 1]))
        move_cursor(terminal, left, true, 1);   
}

void move_word_right(t_input_info *terminal)
{
    while (terminal->index < terminal->length &&
    ft_isspace(terminal->line[terminal->index]))
        move_cursor(terminal, right, true, 1);
    while (terminal->index < terminal->length &&
    !ft_isspace(terminal->line[terminal->index]))
        move_cursor(terminal, right, true, 1);
}

void handle_keys(t_input_info *terminal)
{
    char    buffer[5];

    ft_memset(buffer, 0, 5);
    read(STDIN_FILENO, buffer, 5);
    if (!ft_strncmp(buffer, "[D", 3))
        move_cursor(terminal, left, true, 1);
    else if (!ft_strncmp(buffer, "[C", 3))
        move_cursor(terminal, right, true, 1);    
    else if (!ft_strncmp(buffer, "[H", 3))
        move_cursor(terminal, left, true, terminal->index);
    else if (!ft_strncmp(buffer, "[F", 3) && terminal->index != terminal->length) //Not quite sure why it's needed...
        move_cursor(terminal, right, true, terminal->length - terminal->index);
    else if (!ft_strncmp(buffer, "[A", 3) || !ft_strncmp(buffer, "[B", 3))
		handle_input_history(terminal, buffer[1]);
    else if (!ft_strncmp(buffer, "[3~", 4) && (terminal->index < terminal->length))
    {
        move_cursor(terminal, right, true, 1);
        delete_char(terminal);
    }
}