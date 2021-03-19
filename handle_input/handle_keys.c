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

void handle_keys(t_shell *shell)
{
    char    buffer[1];
    char    aux[5];
    int     i;

    i = 0;
    ft_memset(aux, 0, 5);
    while (read(STDIN_FILENO, buffer, 1))
    {
        aux[i] = buffer[0];
        if (i == 1) //check for ñ, because reasons!
        {
            if (!ft_strncmp(aux, "[D", 3))
            {
                move_cursor(shell, left, true, 1);
                return ;
            }
            else if (!ft_strncmp(aux, "[C", 3))
            {
                move_cursor(shell, right, true, 1);    
                return ;
            }
            else if (!ft_strncmp(aux, "[H", 3))
            {
                move_cursor(shell, left, true, shell->index);
                return ;
            }
            else if (!ft_strncmp(aux, "[F", 3))
            {
                if (shell->index != shell->length) //Not quite sure why it's needed...
                    move_cursor(shell, right, true, shell->length - shell->index);
                return ; 
            }
            else if (!ft_strncmp(aux, "[A", 3) || !ft_strncmp(aux, "[B", 3))
			{
				handle_input_history(shell, aux[1]);
				return ;
			}
        }
        if (i == 2)
        {
            if (!ft_strncmp(aux, "[3~", 4) && (shell->index < shell->length))
            {
                move_cursor(shell, right, true, 1);
                delete_char(shell);
            }
            return ;
        }
        i++;
    }
}