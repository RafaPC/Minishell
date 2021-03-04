/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 19:16:07 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/28 11:21:15 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>

int main(int argc, char **argv, const char **env)
{
	char		*buffer;
	t_list		*env_list;
	t_command	*commands;
	char		**env_array;
	int			prev_exit_status;

	argv++;//CHANGE
	prev_exit_status = errno;
	if (argc == 1) // AQUÍ ENTRA SI LO EJECUTAS NORMAL, SE QUEDA EN BUCLE Y PUEDES METER COMANDOS
	{
		env_list = create_env_list(env);
		env_array = env_list_to_array(env_list);
		while (true)
		{
			printf("Previous exit status: %i\n", prev_exit_status);
			ft_putstr_fd("Minishell-> ", 1);
			read_input(&buffer);
			if (!print_parsing_error(split_commands(&buffer, &commands, env_list))) //TODO add prev_exit_status to parse errors
			{
				while (commands)
					commands = execute_commands(commands, &env_array, &env_list, &prev_exit_status); 
			}
		}
	}
	else // AQUI ENTRA AL LLAMARLO EL DEBUGER, NO PUEDE COGER INPUT POR CONSOLA
	{	// ESCRIBIR LO QUE SE QUIERE EJECUTAR AL DEFINIR EL BUFFER DEBAJO
		env_list = create_env_list((const char **)get_false_env_array());
		env_array = env_list_to_array(env_list);
		buffer = ft_strdup("echo hello");
		if (!print_parsing_error(split_commands(&buffer, &commands, env_list)))
			while (commands)
				commands = execute_commands(commands, &env_array, &env_list, &prev_exit_status);
		free(buffer);
		return (0);
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