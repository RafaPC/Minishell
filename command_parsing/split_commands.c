/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 10:47:52 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/20 18:10:28 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** CMD_PARS = Struct holding cmd pars info.
** -i: index to the current parsing location.
** -j: index to the start of the current argument.
** -error: Error code.
** -error_index: Location wherein an error was found.
**
** INPUT = Parse input.
**
** Given both the input, index and linked list of arguments, mallocs a char **
** with the arguments.
** Note the usage of the binary (cmd_pars->i > cmd_pars->j) to determine the
** number of arguments to malloc. This is of use when i points to command
** separators, as those are not counted as arguments, and thus need no copying.
*/

char	**load_command_args(t_command_parsing *cmd_pars, char *input)
{
	int		argc;
	t_list	*aux;
	char	**command;

	argc = ft_lstsize(cmd_pars->arguments) + (cmd_pars->i > cmd_pars->j);
	if (!(command = malloc((argc + 1) * sizeof(char*))))
		return (0);
	command[argc] = 0;
	argc = 0;
	aux = cmd_pars->arguments;
	while (aux)
	{
		command[argc] = aux->content;
		argc++;
		aux = aux->next;
	}
	ft_lstclear(&cmd_pars->arguments, 0);
	if (cmd_pars->i > cmd_pars->j)
		command[argc] =
		ft_strncpy(&input[cmd_pars->j], cmd_pars->i - cmd_pars->j);
	return (command);
}

/*
** COMMANDS = points to the commands structure. Used to add the parsed command.
**
** ARGUMENTS = Linked list with the different arguments in order.
**
** RELATION = relationship with the next command (see enum).
**
** Adds a command to the command linked list.
** Self explanatory, really
*/

void	add_command(t_command **commands, char **arguments, int relation)
{
	t_command *aux;

	if (!*commands)
	{
		if (!(*commands = ft_calloc(1, sizeof(t_command))))
			return ;
		(*commands)->tokens = arguments;
		(*commands)->relation = relation;
		return ;
	}
	aux = *commands;
	while (aux->next)
		aux = aux->next;
	if (!(aux->next = ft_calloc(1, sizeof(t_command))))
		return ;
	aux->next->tokens = arguments;
	aux->next->relation = relation;
}

/*
** CMD_PARS = Struct holding cmd pars info.
** -i: index to the current parsing location.
** -j: index to the start of the current argument.
** -error: Error code.
** -error_index: Location wherein an error was found.
**
** INPUT = Parse input.
**
** Adds an argument to the linked list "arguments".
** Skips spaces until the next piece of info.
** Note: if i == j it means it has found a space at the beggining of a command,
** and thus it just skips said spaces.
*/

void	add_command_argument(t_command_parsing *cmd_pars, char *input)
{
	if (cmd_pars->i != cmd_pars->j)
		ft_lstadd_back(&cmd_pars->arguments,
		ft_lstnew(ft_strncat_in(0, &input[cmd_pars->j],
		cmd_pars->i - cmd_pars->j)));
	while (ft_isspace(input[cmd_pars->i]))
		cmd_pars->i++;
	cmd_pars->j = cmd_pars->i;
}

/*
** CMD_PARS = Struct holding info about the parsing proccess
**
** COMMANDS = points to the commands structure. Used to add parsed commands.
**
** INPUT = Parse input.
**
** Auxiliary function. Checks for additional parsing operations that involve
** transformation of the string.
** In particular, it checks for quotations and i/o redirections, calling the
** appropiate function.
*/

void	handle_parse_manipulations(t_command_parsing *cmd_pars,
t_shell *shell)
{
	if (shell->buffer[cmd_pars->i] == '\\')
		cmd_pars->i = handle_backslash(&shell->buffer, cmd_pars->i, false);
	else if (ft_strrchr("\"\'", shell->buffer[cmd_pars->i]))
		skip_quotations(shell->buffer, cmd_pars);
	else if ((shell->buffer[cmd_pars->i] == '<'))
		handle_input_redirection(cmd_pars, &shell->commands, &shell->buffer);
	else if (shell->buffer[cmd_pars->i] == '>')
		handle_redirections_split(cmd_pars, &shell->commands, &shell->buffer);
	else if (ft_isspace(shell->buffer[cmd_pars->i]))
		add_command_argument(cmd_pars, shell->buffer);
	else
		cmd_pars->i++;
}

/*
** INPUT = Parse input.
**
** COMMANDS = points to the commands structure. Used to add parsed commands.
**
** Receives a malloced string "input" from which to parse the commands.
** Simple while loop that checks for transformations and special characters
** Does modify the input string, freeing the old one and mallocing the new one
** Returns an int of value 0 on success and an error code based on what went
** wrong.
**
** NOTE: Assumes that *input is malloced.
*/

int		split_commands(t_shell *shell)
{
	t_command_parsing	cmd_pars;

	shell->commands = NULL;
	ft_memset(&cmd_pars, 0, sizeof(cmd_pars));
	while (true)
	{
		if (cmd_pars.error)
		{
			free_commands(shell->commands);
			shell->commands = NULL;
			free(shell->buffer);
			return (cmd_pars.error);
		}
		else if (ft_strchr(";|", (shell->buffer)[cmd_pars.i]))
		{
			if (command_split(&cmd_pars, &shell->commands, shell->buffer))
			{
				free(shell->buffer);
				return (0);
			}
		}
		else
			handle_parse_manipulations(&cmd_pars, shell);
	}
	return (0);
}
