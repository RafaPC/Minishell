/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 19:33:45 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/26 21:03:53 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_commands(t_command *commands)
{
	t_command *aux;

	while (commands)
	{
		aux = commands;
		ft_array_clear((void *)commands->tokens, free);
		commands = commands->next;
		free(aux);
	}
}