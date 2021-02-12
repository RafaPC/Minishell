#include "libft.h"
#include "minishell.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	char		*buffer;
	t_list		*envp_list;
	t_command	*commands;
	char		*aux;

	commands = (t_command*)malloc(sizeof(t_command));
	envp_list = create_env_list((const char **)get_false_env_list());

	if (argc == 1) // AQUÍ ENTRA SI LO EJECUTAS NORMAL, SE QUEDA EN BUCLE Y PUEDES METER COMANDOS
	{
		while (true)
		{
			buffer = (char*)malloc(sizeof(char) * 1025);
			read_input(buffer);
			tabs_to_spaces(buffer);
			aux = buffer;
			buffer = ft_strtrim(buffer, " ");
			free(aux);
			split_commands(buffer, &commands);
			for (t_command *temp = commands; temp; temp = temp->next)
				printf("Comando: %s - Relacion: %i\n", *(temp->command), temp->relation);
			empty_buffer(buffer);
		}
	}
	else // AQUI ENTRA AL LLAMARLO EL DEBUGER, NO PUEDE COGER INPUT POR CONSOLA
	{	// ESCRIBIR LO QUE SE QUIERE EJECUTAR AL DEFINIR EL BUFFER DEBAJO
		buffer = "hola ; holo";
		split_commands(buffer, &commands);
		// PARA VER COMO QUEDA LA LISTA DE COMMANDS
		for (t_command *aux = commands; aux; aux = aux->next)
		{
			printf("Comando: %s - Relacion: %i\n", *(aux->command), aux->relation);
		}
		return (0);
	}
}
