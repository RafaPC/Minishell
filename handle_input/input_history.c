/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 20:22:18 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/19 12:55:49 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" 

 void   rewrite_line(t_shell *shell, char *line, t_bool erase_mode, t_bool duplicate_string)
 {
    if (erase_mode)
        free(shell->line);
    if (duplicate_string)
        shell->line = ft_strdup(shell->current_history->content);
    else
        shell->line = line;
    shell->index = ft_strlen(shell->line);
    shell->length = shell->index;
    ft_putstr_fd("\r\33[0J", STDIN_FILENO); // TODO maybe: mover el cursor solo hasta index == no tener que reescribir el prompt
    write_prompt();
    ft_putstr_fd(shell->line, STDIN_FILENO);
 }

void    retrieve_older_command(t_shell *shell)
{
    if (shell->current_history)
    {
        if (!shell->current_history->prev && !shell->h_saved_line)
        {
            shell->h_saved_line = shell->line;
            rewrite_line(shell, shell->current_history->content, false, true);
        }
        else
        {
            if (shell->current_history->next)
                shell->current_history = shell->current_history->next;
            rewrite_line(shell, shell->current_history->content, true, true);
        }
    }
}

void    retrieve_newer_command(t_shell *shell)
{
    if (shell->current_history)
    {
        if (!shell->current_history->prev)
        {
            if (shell->h_saved_line)
            {
                rewrite_line(shell, shell->h_saved_line, false, false);
                shell->h_saved_line = 0;
            }
        }
        else
        {
            shell->current_history = shell->current_history->prev;
            rewrite_line(shell, shell->current_history->content, true, true);
        }
    }
}

void handle_input_history(t_shell *shell, char direction)
{
    if (direction == 'A')
        retrieve_older_command(shell);
    else
        retrieve_newer_command(shell);
}