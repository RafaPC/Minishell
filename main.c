/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 19:16:07 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/09 12:18:53 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>

// #include <signal.h>
// void	signal_interrump(int signal)
// {
// 	ft_putstr_fd("\nPrompt:", 1);
// }

int main(int argc, char **argv, const char **env)
{
	char		*buffer;
	t_list		*env_list;
	t_command	*commands;
	int			prev_exit_status;

	// signal(SIGINT, signal_interrump);
	prev_exit_status = errno;
	env_list = create_env_list(env);
	if (argc == 1) // AQUÍ ENTRA SI LO EJECUTAS NORMAL, SE QUEDA EN BUCLE Y PUEDES METER COMANDOS
	{
		while (true)
		{
			ft_putstr_fd("Minishell-> ", 1);
			read_input(&buffer);
			if (!print_parsing_error(split_commands(&buffer, &commands), &prev_exit_status))
			{
				while (commands)
					commands = execute_commands(commands, &env_list, &prev_exit_status); 
			}
		}
	}
	else if (argc == 2)
		debug_minishell(&env_list, !ft_strncmp(argv[1], "-v", 3));		
	else //PARA EL TESTER, COGE EL INPUT POR EL ARGUMENTO
	{
		buffer = ft_strdup(argv[argc - 1]);
		if (!print_parsing_error(split_commands(&buffer, &commands), &prev_exit_status))
			while (commands)
				commands = execute_commands(commands, &env_list, &prev_exit_status);
		return (prev_exit_status);
	}
}