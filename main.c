/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 19:16:07 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/07 11:53:23 by aiglesia         ###   ########.fr       */
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
	char		**env_array;
	int			prev_exit_status;

	// signal(SIGINT, signal_interrump);
	prev_exit_status = errno;
	if (argc == 1) // AQUÍ ENTRA SI LO EJECUTAS NORMAL, SE QUEDA EN BUCLE Y PUEDES METER COMANDOS
	{
		env_list = create_env_list(env);
		env_array = env_list_to_array(env_list);
		while (true)
		{
			ft_putstr_fd("Minishell-> ", 1);
			read_input(&buffer);
			if (!print_parsing_error(split_commands(&buffer, &commands), &prev_exit_status)) //TODO add prev_exit_status to parse errors
			{
				while (commands)
					commands = execute_commands(commands, &env_array, &env_list, &prev_exit_status); 
			}
		}
	}
	else if (argc == 2) // AQUI ENTRA AL LLAMARLO EL DEBUGER, NO PUEDE COGER INPUT POR CONSOLA
	{	// ESCRIBIR LO QUE SE QUIERE EJECUTAR AL DEFINIR EL BUFFER DEBAJO
		env_array = get_false_env_array();
		env_list = create_env_list((const char**)env_array);
		buffer= ft_strdup(">>> a");
		if (!print_parsing_error(split_commands(&buffer, &commands), &prev_exit_status))
			while (commands)
				commands = execute_commands(commands, &env_array, &env_list, &prev_exit_status);
		free(buffer);
		return (0);
	}
	else //PARA EL TESTER, COGE EL INPUT POR EL ARGUMENTO
	{
		env_list = create_env_list(env);
		env_array = env_list_to_array(env_list);
		buffer = ft_strdup(argv[argc - 1]);
		//buffer = ft_strdup("exit 17");
		if (!print_parsing_error(split_commands(&buffer, &commands), &prev_exit_status))
			while (commands)
				commands = execute_commands(commands, &env_array, &env_list, &prev_exit_status);
		free(buffer);
		return (prev_exit_status);
	}
}

		// PARA VER COMO QUEDA LA LISTA DE COMMANDS
		// for (t_command *aux = commands; aux; aux = aux->next)
		// {
		// 		command_reader = aux->tokens;
 		// 		printf("Comando: %s - Relacion: %i\nArgumentos:\n", *command_reader, aux->relation);
		// 		while (*(++command_reader))
		// 			printf("%s\n", *command_reader);
		// }