/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 20:24:45 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/19 13:06:20 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

 void   delete_h_saved_line(t_shell *shell)
 {
    if (shell->h_saved_line)
    {
        free(shell->h_saved_line);
        shell->h_saved_line = 0;
    }
 }

void move_cursor(t_shell *shell, int direction, t_bool change_index, unsigned nb)
{
    if (nb == 0)
        return ;
    if (direction == left && shell->index)
    {
        if (nb > shell->index)
            nb = shell->index;
        if (change_index)
            shell->index -= nb;
        ft_printf(STDIN_FILENO, "\33[%iD", nb);
    }
    else if (direction == right && shell->index < shell->length)
    {
        if (nb > shell->length - shell->index)
            nb = shell->length - shell->index;
        if (change_index)
            shell->index += nb;
        ft_printf(STDIN_FILENO, "\33[%iC", nb);
    }
}

void    delete_char(t_shell *shell)
{
    int index;

    if (shell->index)
    {
        ft_putstr_fd("\b\33[0K", STDIN_FILENO);
        (shell->index)--;
        (shell->length)--;
        ft_extract(&shell->line, shell->index, 1);
        if (shell->index != shell->length)
        {
            ft_putstr_fd(&shell->line[shell->index], STDIN_FILENO); 
            index = shell->index;
            shell->index = shell->length;
            move_cursor(shell, left, true, shell->length - index);
        }
    }
    delete_h_saved_line(shell);
}

void    add_char(t_shell *shell, char c)
{
    char    buffer[2];
    int     index;

    buffer[0] = c;
    buffer[1] = '\0';
    ft_insert(&shell->line, buffer, shell->index, 1);
    (shell->index)++;
    (shell->length)++;
    ft_putstr_fd(&shell->line[shell->index - 1], STDIN_FILENO);
    if (shell->index != shell->length)
    {
        index = shell->index;
        shell->index = shell->length;
        move_cursor(shell, left, true, shell->length - index);
    }
    delete_h_saved_line(shell);
}


t_bool    read_input(t_shell *shell)
{
    char    buffer[1];
    
    while (read(STDIN_FILENO, buffer, 1))
    {
        if (buffer[0] == end_of_transmission && !(shell->line[0])) // Add Ctrl + c signal?
            return (false);
        if (buffer[0] == '\n')
            break ;
        else if (buffer[0] == escape)
            handle_keys(shell);
        else if (buffer[0] == delete)
            delete_char(shell);
        else
            add_char(shell, buffer[0]);
    }
    write(STDIN_FILENO, "\n", 1);
    if (*(shell->line) && (!shell->history ||
    ft_strncmp(shell->line, shell->history->content, shell->length)))
        ft_lstdbl_add_front(&shell->history, ft_lstdbl_new(ft_strdup(shell->line)));
    delete_h_saved_line(shell);
}
