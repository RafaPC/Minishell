/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 10:19:14 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/14 12:43:41 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_quotations(char **input, t_command_parsing *cmd_pars)
{
	char *closer;
	char *alternate;

	closer = input[0][cmd_pars->i] == '\"' ? "\"" : "\'";
	alternate = input[0][cmd_pars->i] == '\'' ? "\"" : "\'";
	cmd_pars->i++;
	while (!ft_strchr(closer, input[0][cmd_pars->i]))
	{
		if (!ft_strncmp(&input[0][cmd_pars->i], "\\\"", 2) ||
		!ft_strncmp(&input[0][cmd_pars->i], "\\\'", 2))
			cmd_pars->i += 2;
		if (ft_strchr(alternate, input[0][cmd_pars->i]))
			skip_quotations(input, cmd_pars);
		else
			cmd_pars->i++;
	}
	if (!input[0][cmd_pars->i])
		cmd_pars->error = newline;
	else
		cmd_pars->i++;
}

/*
** INPUT = Parse input.
**
** I = Index to the current parsing location.
**
** Same as the double quote function, with the exception
** that no conversions are done.
*/

void	handle_single_quotations(char **input, int *index)
{
	*index = ft_extract(input, *index, 1);
	while ((*input)[*index])
	{
		if ((*input)[*index] == '\'')
		{
			*index = ft_extract(input, *index, 1);
			return ;
		}
		(*index)++;
	}
}

int		handle_backslash(char **args, int index, t_bool remove)
{
	if (remove)
		index = ft_extract(args, index, 1);
	else
		index++;
	if (ft_strrchr("\"\'$\\", args[0][index]))
		index++;
	return (index);
}

/*
** INPUT = Parse input.
**
** I = Index to the current parsing location.
**
** Handles double quotation transformation.
** Thus it only checks for '$' and '"' for any special behaviour.
** Removes the initial and last quotation (if found), returning an
** index to the character preceding the last quotation mark.
** Therefore, given how 'j' in the super function is located at the beggining
** of the argument, the complete quoted content shall be copied as an argument
**
** Might consider stopping the parsing and printing an error
** if no closing quotation is found.
*/

void	handle_double_quotations(char **input, int *index,
t_list *env_list, int prev_exit_status)
{
	*index = ft_extract(input, *index, 1);
	while ((*input)[*index])
	{
		if (input[0][*index] == '\\')
			*index = handle_backslash(input, *index, true);
		else if ((*input)[*index] == '$')
			*index = insert_variable(input, *index,
			env_list, prev_exit_status);
		else if ((*input)[*index] == '\"')
		{
			*index = ft_extract(input, *index, 1);
			return ;
		}
		else
			(*index)++;
	}
}

/*
** INPUT = Parse input.
**
** I = Index to the current parsing location.
**
** Buffer function as an intermediary to call the respective function.
** Consider removal if enough space in the father function.
*/

void	handle_quotations(char **input, int *index,
t_list *env_list, int prev_exit_status)
{
	if ((*input)[*index] == '\"')
		handle_double_quotations(input, index, env_list, prev_exit_status);
	else if ((*input)[*index] == '\'')
		handle_single_quotations(input, index);
}
