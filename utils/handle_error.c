/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/27 19:37:11 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/08 12:33:01 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>

t_command	*handle_errors(t_command *command)
{
	if (errno == 2)
		ft_printf("minishell: %s: %s\n", command->tokens[0], strerror(errno));
	else
		ft_printf("minishell: %s\n", strerror(errno));
	while (command->relation != simple_command)
		command = del_command(command);
	command = del_command(command);
	return (command);
}

t_command	*print_redirection_errors(t_command *commands, int *prev_exit_status) //What about dup?
{
	if (errno == 2)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(commands->tokens[0], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	}
	if (errno == 21)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(commands->tokens[0], STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
	}
	while (commands->relation != simple_command)
		commands = del_command(commands);
	*prev_exit_status = 1; //For some reason...
	return (commands);
}
