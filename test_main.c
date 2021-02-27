/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 19:16:07 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/27 01:40:29 by aiglesia         ###   ########.fr       */
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

	if (argc == 1) // AQUÍ ENTRA SI LO EJECUTAS NORMAL, SE QUEDA EN BUCLE Y PUEDES METER COMANDOS
	{
		env_list = create_env_list(env);
		env_array = env_list_to_array(env_list);
		while (true)
		{
			ft_putstr_fd("Minishell-> ", 1); //TODO change so it dynamically allocates things?
			read_input(&buffer);
			if (!print_parsing_error(split_commands(&buffer, &commands, env_list)))
			{
				if (commands->tokens[0] && !ft_strncmp(*(commands->tokens), "exit", 5)) //Change later on
					break;
				execute_commands(commands, &env_array, env_list);
			}
			free_commands(commands);
			commands = NULL;
			free(buffer);
		}
		free_commands(commands);
		ft_lstclear(&env_list, free);
		free(buffer);
		ft_array_clear((void*)env_array, free);
	}
	else // AQUI ENTRA AL LLAMARLO EL DEBUGER, NO PUEDE COGER INPUT POR CONSOLA
	{	// ESCRIBIR LO QUE SE QUIERE EJECUTAR AL DEFINIR EL BUFFER DEBAJO
		env_list = create_env_list((const char **)get_false_env_array());
		env_array = env_list_to_array(env_list);
		buffer = ft_strdup("clear");
		if (!print_parsing_error(split_commands(&buffer, &commands, env_list)))
			execute_commands(commands, &env_array, env_list);
		free_commands(commands);
		free(buffer);
		buffer = ft_strdup("echo hello");
		split_commands(&buffer, &commands, env_list);
		env_array = env_list_to_array(env_list);
		execute_commands(commands, &env_array, env_list);
		free_commands(commands); // CHange with function!
		ft_lstclear(&env_list, free);
		free(buffer);
		// PARA VER COMO QUEDA LA LISTA DE COMMANDS
		// for (t_command *aux = commands; aux; aux = aux->next)
		// {
		// 		command_reader = aux->tokens;
 		// 		printf("Comando: %s - Relacion: %i\nArgumentos:\n", *command_reader, aux->relation);
		// 		while (*(++command_reader))
		// 			printf("%s\n", *command_reader);
		// }
		return (0);
	}
}