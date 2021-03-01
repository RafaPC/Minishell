/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/27 19:37:11 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/27 20:27:27 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>

t_command	*handle_errors(t_command *command)
{
	if (errno == 2)
		ft_printf("%s: %s\n", strerror(errno), command->tokens[0]);	
	else
		ft_printf("%s", strerror(errno));	
	while (command->relation != simple_command)
		command = del_command(command);
	command = del_command(command);
	return (command);
}