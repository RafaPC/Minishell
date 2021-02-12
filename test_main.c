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
			ft_putstr_fd("Minishell-> ", 1); //TODO change so it dynamically allocates things?
			get_next_line(1, &buffer);
			split_commands(buffer, &commands);
			if (!ft_strncmp(*(commands->command), "exit", 5)) //Change later on
				break;
			for (t_command *temp = commands; temp; temp = temp->next)
				printf("Comando: %s - Relacion: %i\n", *(temp->command), temp->relation);
			free(buffer);
		}
	}
	else // AQUI ENTRA AL LLAMARLO EL DEBUGER, NO PUEDE COGER INPUT POR CONSOLA
	{	// ESCRIBIR LO QUE SE QUIERE EJECUTAR AL DEFINIR EL BUFFER DEBAJO
		buffer = "hello > test";
		split_commands(buffer, &commands);
		// PARA VER COMO QUEDA LA LISTA DE COMMANDS
		for (t_command *aux = commands; aux; aux = aux->next)
		{
			printf("Comando: %s - Relacion: %i\n", *(aux->command), aux->relation);
		}
		return (0);
	}
}
