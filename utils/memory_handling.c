/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 19:33:45 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/20 13:47:53 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** TODO:
*/

t_command	*del_command(t_command *command)
{
	t_command *aux;

	aux = command;
	if (!command)
		return (NULL);
	ft_array_clear((void *)command->tokens, free);
	command = command->next;
	free(aux);
	return (command);
}

/*
** TODO:
*/

void		free_commands(t_command *commands)
{
	while (commands)
		commands = del_command(commands);
}
