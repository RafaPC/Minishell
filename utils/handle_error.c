/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/27 19:37:11 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/08 15:49:03 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>

t_command	*handle_errors(t_command *command)
{
	if (errno == 2)
		ft_printf(STDERR_FILENO,
		"minishell: %s: %s\n", command->tokens[0], strerror(errno));
	else
		ft_printf(STDERR_FILENO, "minishell: %s\n", strerror(errno));
	while (command->relation != simple_command)
		command = del_command(command);
	command = del_command(command);
	return (command);
}

t_command	*print_redirection_errors(
	t_command *commands, int *prev_exit_status)
{
	if (errno == 2)
		ft_printf(STDERR_FILENO,
		"minishell: %s: No such file or directory\n", commands->tokens[0]);
	if (errno == 21)
		ft_printf(STDERR_FILENO,
		"minishell: %s: Is a directory\n", commands->tokens[0]);
	while (commands->relation != simple_command)
		commands = del_command(commands);
	*prev_exit_status = 1;
	return (commands);
}
