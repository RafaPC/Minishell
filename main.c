/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 19:16:07 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/20 20:01:39 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int		main(int argc, char **argv, const char **env)
{
	t_shell		shell;

	shell.prev_exit_status = errno;
	shell.env_list = create_env_list(env, argv[0]);
	shell.command_history = NULL;
	shell.commands = NULL;
	shell.copy_line = NULL;
	if (argc == 1)
	{
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
	else if (argc == 2)
	{
		ft_lstclear(&shell.env_list, free); //FIXME: todo este bloque es temporal, borrar antes de entregar
		shell.env_list = create_env_list((const char**)get_false_env_array(), argv[0]);
		debug_minishell(&shell, !ft_strncmp(argv[1], "-v", 3));
	}
	else //PARA EL TESTER, COGE EL INPUT POR EL ARGUMENTO
	{
		shell.buffer = ft_strdup(argv[argc - 1]);
		if (!print_parsing_error(split_commands(&shell), &shell.prev_exit_status))
			while (shell.commands)
				shell.commands = execute_commands(&shell);
		return (shell.prev_exit_status);
	}
}
