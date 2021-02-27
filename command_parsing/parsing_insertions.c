/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_insertions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 11:41:44 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/27 11:26:17 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** INPUT = Parse input.
**
** I = Index to the current parsing location.
**
** Same as the double quote function, with the exception
** that no conversions are done.
*/

void	handle_single_quotations(char **input, t_command_parsing *cmd_pars)
{
	cmd_pars->i = ft_extract(input, cmd_pars->i, 1);
	while ((*input)[cmd_pars->i])
	{
		if ((*input)[cmd_pars->i] == '\'')
		{
		cmd_pars->i = ft_extract(input, cmd_pars->i, 1);
		return ;
		}
		cmd_pars->i++;
	}
		cmd_pars->error = 5;
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

void	handle_double_quotations(char **input, t_command_parsing *cmd_pars, t_list *env_list)
{
	cmd_pars->i = ft_extract(input, cmd_pars->i, 1);
	while ((*input)[cmd_pars->i])
	{
		if ((*input)[cmd_pars->i] == '\\')
		{
			if (ft_strchr("$`\"", (*input)[cmd_pars->i + 1]))
				cmd_pars->i = ft_extract(input, cmd_pars->i, 1) + 1;
			else
				cmd_pars->i++;
		}
		if ((*input)[cmd_pars->i] == '$')
			cmd_pars->i = insert_variable(input, cmd_pars->i, env_list);
		if ((*input)[cmd_pars->i] == '\"')
		{
		cmd_pars->i = ft_extract(input, cmd_pars->i, 1);
		return ;
		}
		else
			cmd_pars->i++;
	}
		cmd_pars->error = 6;
}

/*
** INPUT = Parse input.
**
** I = Index to the current parsing location.
**
** Buffer function as an intermediary to call the respective function.
** Consider removal if enough space in the father function.
*/

void	handle_quotations(char **input, t_command_parsing *cmd_pars, t_list *env_list)
{
	if ((*input)[cmd_pars->i] == '\"')
		handle_double_quotations(input, cmd_pars, env_list);
	else if ((*input)[cmd_pars->i] == '\'')
		handle_single_quotations(input, cmd_pars);
}

/*
**INPUT = Parse input.
**
** Index = Index to the '$'.
**
** Called when a '$' is found. It removes said '$' and any
** alphanumeric and '_' characters that may preceed it.
** Said characters are saved to a temporal string to search
** for any stored variable that may match it.
** If one is found, the value of said variable is inserted.
** In any case, an index is returned pointing to the char
** preceeding the insertion (or lack thereof);
*/

int	insert_variable(char **input, int index, t_list *env_list) //Add t_list *envp_list
{
	int		j;
	char	*aux;
	char	*variable;

	j = 1;
	while (ft_isalnum((*input)[index + j]) || (*input)[index + j] == '_')
		j++;
	if (!(aux = ft_strncpy(&(*input)[index + 1], j)))
		return (index);
	variable = get_env_var(aux, env_list);
	index = ft_extract(input, index + j - 1, j);
	if (variable)
		index = ft_insert(input, variable, index, ft_strlen(variable));
	free(aux);
	return (index);
	
}
