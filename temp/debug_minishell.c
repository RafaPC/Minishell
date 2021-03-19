/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 11:28:26 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/19 10:48:57 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_command_table(t_command *command)
{
	char	**token_reader;

	while (true)
	{
		token_reader = command->tokens;
		ft_printf(STDOUT_FILENO,
			"Comando: %s\n\nRelacion: %i\n\nArgumentos:\n\n",
			token_reader[0], command->relation);
		while ((++token_reader)[0])
			ft_printf(STDOUT_FILENO, "%15.d%c %s\n", 0, '*', token_reader[0]);
		ft_printf(STDOUT_FILENO, "\nOutput:\n\n");
		if (command->relation == 0)
			break ;
		command = command->next;
	}
}

void	iterate_through_commands(t_gnl_buffer *gnl_buff,
t_shell *shell, t_bool verbose)
{
	char		*buffer;
	t_command	*commands;
	int			prev_exit_status;

	commands = NULL;
	while (gnl_buff)
	{
		buffer = gnl_buff->line;
		ft_printf(STDOUT_FILENO, "Input: \"%s\"\n\n", buffer);
		if (!print_parsing_error(
			split_commands(shell), &prev_exit_status))
		{
			while (commands)
			{
				if (verbose)
					print_command_table(commands);
				commands = execute_commands(shell);
			}
		}
		gnl_buff = gnl_buff->next;
		ft_printf(STDOUT_FILENO, "---\n");
	}
}

void	debug_minishell(t_shell *shell, t_bool verbose)
{
	int				fd;
	t_gnl_buffer	*gnl_buff;

	gnl_buff = NULL;
	fd = open("command_list", O_RDONLY);
	if (fd != -1)
	{
		gnl_buffer(fd, 0, &gnl_buff);
		iterate_through_commands(gnl_buff, shell, verbose);
		free_gnl_buffer(gnl_buff, false);
		close(fd);
	}
	exit_command(shell, 0, 0); //double check
}
