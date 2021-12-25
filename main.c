/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 19:16:07 by aiglesia          #+#    #+#             */
/*   Updated: 2021/12/25 16:59:20 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int		main(int argc, char **argv, const char **env)
{
	t_shell		shell;

	shell.prev_exit_status = errno;
	shell.command_history = NULL;
	shell.commands = NULL;
	shell.copy_line = NULL;
	if (argc == 1)
	{
		shell.env_list = create_env_list(env, argv[0]);
		while (true)
		{
			if (!handle_input(&shell.buffer, &shell.command_history, &shell.copy_line))
			{
				write(STDOUT_FILENO, "exit\n", 5);
				free_and_exit(&shell, 130);
			}
			if (!print_parsing_error(split_commands(&shell), &shell.prev_exit_status))
			{
				while (shell.commands)
					shell.commands = execute_commands(&shell);
			}
		}
	}
}