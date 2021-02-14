/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 12:03:51 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/14 09:50:33 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** CMD_PARS = Struct holding cmd pars info.
** -i: index to the location of the pipe.
** -j: index to the start of the current argument.
** -error: Error code.
** -error_index: Location wherein an error was found.
**
** COMMANDS = points to the commands structure. Used to add the parsed command.
**
**INPUT = Parse input.
**
** Self explanatory name.
** If a pipe immediately follows the current one, an error is issued 
** Additionally, the redirections must be followed by some sort of alphanumeric content.
** Therefore, if only spaces are found preceding the pipe, an error is issued.
**
**  Should an error be found, the linked argument list is freed.
**  The commands already malloced should be freed by the super function.
*/

void	handle_pipe_split(t_command_parsing *cmd_pars,
t_command **commands, char *input)
{
	int		counter_aux;
	int		error;

	counter_aux = 0;
	while (ft_isspace(input[cmd_pars->i + 1 + counter_aux]))
		counter_aux++;
	error = (!input[cmd_pars->i + 1 + counter_aux] ||
	input[cmd_pars->i + 1] == '|') ? 1 : 0;
	if (error)
	{
		ft_lstclear(&cmd_pars->arguments, free);
		cmd_pars->error = 1;
		cmd_pars->error_index = cmd_pars->i;
		return ;
	}
	add_command(commands, load_command_args(cmd_pars, input), 3);
	cmd_pars->i += counter_aux + 1;
	cmd_pars->j = cmd_pars->i;
}

char	**get_redirection_command(t_command_parsing *cmd_pars, char **input, int index)
{
	int lenght;
	char **command_args;
	int start;
	int end;

	lenght = 0;
	start = cmd_pars->i;
	cmd_pars->i = index;
	while (!(ft_isspace((*input)[cmd_pars->i]) || !(*input)[cmd_pars->i]))
	{
		if (ft_strchr("\"\'", (*input)[cmd_pars->i]))
			handle_quotations(input, cmd_pars);
		else if ((*input)[cmd_pars->i] == '$')
			insert_variable(input, cmd_pars->i);
		else
			cmd_pars->i++;
	}
	lenght = cmd_pars->i - index;
	end = index + lenght;
	if (!(command_args = ft_alloc(2, sizeof(char *))))
	{
		cmd_pars->i = ft_extract(input, end - 1, end - start);
		return (0);
	}
	*command_args = ft_strncpy(&(*input)[index], lenght);
	cmd_pars->i = ft_extract(input, end - 1, end - start);
	return (command_args);
}

/*
** CMD_PARS = Struct holding cmd pars info.
** -i: index to the location of the redirection.
** -j: index to the start of the current argument.
** -error: Error code.
** -error_index: Location wherein an error was found.
**
** COMMANDS = points to the commands structure. Used to add the parsed command.
**
**INPUT = Parse input.
**
** Self explanatory name.
** Keep in mind this handles both '>' and '>>' redirections.
** To do so, a counter is used to set the appropiate relation, by counting the amount of adyacent '>'.
** Therefore, '>' = 1, and '>>' = 2 (duh). If the counter > 3, an error is issued.
** Additionally, the redirections must be followed by some sort of alphanumeric content.
** Therefore, if only spaces are found preceding the '>', an error is issued.
**
**  Should an error be found, the linked argument list is freed.
**  The commands already malloced should be freed by the super function.
*/

void	handle_redirections_split(t_command_parsing *cmd_pars,
t_command **commands, char **input)
{
	int		counter;
	int		counter_aux;
	int		error;
	char	**command;

	counter = 1;
	counter_aux = 0;
	while ((*input)[cmd_pars->i + counter] == '>')
		counter++;
	while (ft_isspace((*input)[cmd_pars->i + counter + counter_aux]))
		counter_aux++;
	error = (!(*input)[cmd_pars->i + counter + counter_aux] ||
	counter > 2) ? 1 : 0;
	if (error)
	{
		ft_lstclear(&cmd_pars->arguments, free);
		cmd_pars->error = 1;
		cmd_pars->error_index = cmd_pars->i + counter;
		return ;
	}
	command = get_redirection_command(cmd_pars, input,
	cmd_pars->i + counter + counter_aux);
	add_command(commands, command, counter + 1);
}

/*
** CMD_PARS = Struct holding cmd pars info.
** -i: index to the location of the ';' / NULL.
** -j: index to the start of the current argument.
** -error: Error code.
** -error_index: Location wherein an error was found.
**
** COMMANDS = points to the commands structure. Used to add the parsed command.
**
**INPUT = Parse input.
**
** Adds a command structure with the stored arguments to the command linked list.
** Sets the i to skip the ';' if needded, also setting j to the i value, so it may copy the following argument.
** Note that, since in the main while spaces are skipped if i and j are the same, should a ; be preceeded by just spaces,
** said spaces would be ignored when continuing the parse.
** 
** Aditionally, if a ';' is find, two parsing checks are done.
** 
** 	Firstly, it checks that the ';' is followed by alphanumeric information, by moving backwards until something other than a space is found.
** 	Therefore, should aux get to 0, or should it point to a ';', this check would be considered as failed.
** 
** 	Secondly, it checks that the ';' is not immediately followed by another one.
** 
** 	Note that if something like this were to happen ("; ;"), a parsing error would still be handled, as this will fail the first check when done on the second ';'
**
**  Should an error be found, the linked argument list is freed.
**  The commands already malloced should be freed by the super function.
**
*/

void	handle_simple_command_split(t_command_parsing *cmd_pars,
t_command **commands, char *input)
{
	int		aux;

	aux = cmd_pars->i - 1;
	if (input[cmd_pars->i] == ';')
	{
		while (aux > 0 && ft_isspace(input[aux]))
			aux--;
		if (input[cmd_pars->i + 1] == ';' || input[aux] == ';' || aux == 0)
		{
			ft_lstclear(&cmd_pars->arguments, free);
			cmd_pars->error = 1;
			cmd_pars->error_index = aux;
			return ;
		}
	}
	add_command(commands, load_command_args(cmd_pars, input), 0);
	if (input[cmd_pars->i])
		cmd_pars->i += 1;
	cmd_pars->j = cmd_pars->i;
}

/*
** CMD_PARS = Struct holding cmd pars info.
** -i: index to the current parsing location.
** -j: index to the start of the current argument.
** -error: Error code.
** -error_index: Location wherein an error was found.
**
** COMMANDS = points to the commands structure. Used to add the parsed command.
**
**INPUT = Parse input.
**
** Hub function for the command splitting function.
** If any of the special characters are found, it calls the appropiate function.
** Note that in the case of simple command splittage, it checks for both a ';' and the end of the string.
** Aditionally, said command is only executed if i != j. This prevents empty commands when said indexes are set to the null character.
** I.e: Spaces following a ';'
** Returns true if input[cmd_pars->i] points at NULL and thus no more parsing is required.
** Note that this will also end the parent function.
*/

t_bool	command_split(t_command_parsing *cmd_pars,
t_command **commands, char *input)
{
	if (!(input[cmd_pars->i]) || input[cmd_pars->i] == ';')
		handle_simple_command_split(cmd_pars, commands, input);
	if (input[cmd_pars->i] == '|')
		handle_pipe_split(cmd_pars, commands, input);
	return (input[cmd_pars->i] ? false : true);
}
