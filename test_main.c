#include "libft.h"
#include "minishell.h"
#include <stdio.h>

int main(int argc, char **argv, const char **env)
{
	char		*buffer;
	t_list		*env_list;
	t_command	*commands;
	char		**env_array;

	commands = NULL;
	if (argc == 1) // AQUÍ ENTRA SI LO EJECUTAS NORMAL, SE QUEDA EN BUCLE Y PUEDES METER COMANDOS
	{
		env_list = create_env_list(env);
		env_array = env_list_to_array(env_list);
		while (true)
		{
			ft_putstr_fd("Minishell-> ", 1); //TODO change so it dynamically allocates things?
			get_next_line(1, &buffer);
			// buffer = ft_strdup("< hello echo");
			split_commands(&buffer, &commands); // TODO Check return value to see if there was a parsing error.
 			if (commands->tokens[0] && !ft_strncmp(*(commands->tokens), "exit", 5)) //Change later on
 				break;
 			execute_commands(commands, &env_array, env_list);
			// for (t_command *temp = commands; temp; temp = temp->next)
			// {
			// 	command_reader = temp->tokens;
 			// 	printf("Comando: %s - Relacion: %i\n", *command_reader, temp->relation);
			// 	if (*(command_reader + 1))
			// 	{
			// 		printf("%2sArgumentos:\n", "");
			// 		while (*(++command_reader))
			// 			printf("%15s%s\n", "", *command_reader);
			// 	}
			// 	printf("\n");
			// }
			
			free(buffer);
		}
	}
	else // AQUI ENTRA AL LLAMARLO EL DEBUGER, NO PUEDE COGER INPUT POR CONSOLA
	{	// ESCRIBIR LO QUE SE QUIERE EJECUTAR AL DEFINIR EL BUFFER DEBAJO
		env_list = create_env_list((const char **)get_false_env_array());
		buffer = ft_strdup("echo -n hola");
		split_commands(&buffer, &commands);
		env_array = env_list_to_array(env_list);
		execute_commands(commands, &env_array, env_list);
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