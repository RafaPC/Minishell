/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_insertions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 11:41:44 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/15 20:50:53 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		handle_variable_insertion(char **input, int index, t_list *env_list)
{
	int		j;
	char	*variable;
	char	*aux;

	j = 0;
	while (ft_isalnum((*input)[index + j + 1]) ||
	(*input)[index + j + 1] == '_')
		j++;
	if (!j)
		return (index + 1);
	if (!(aux = ft_strncpy(&(*input)[index + 1], j)))
		return (index);
	variable = get_env_var(aux, env_list);
	index = ft_extract(input, index + j, j + 1);
	index = ft_insert(input, variable, index, ft_strlen(variable));
	if (variable)
		free(variable);
	free(aux);
	return (index);
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

int		insert_variable(char **input, int index, t_list *env_list,
int prev_exit_status)
{
	char	*aux;

	if (ft_isspace((*input)[index + 1]) || !(*input)[index + 1])
		return (index + 1);
	if (!ft_strncmp("$?", &(*input)[index], 2))
	{
		aux = ft_itoa(prev_exit_status);
		index = ft_extract(input, index + 1, 2);
		index = ft_insert(input, aux, index, ft_strlen(aux));
		free(aux);
	}
	else
		return (handle_variable_insertion(input, index, env_list));
	return (index);
}

void	parse_insertions(char **args, t_list *env_list,
int prev_exit_status, t_bool single_run) //TODO: when bored: Change cmd_pars to simple int;
{
	int index;

	index = 0;
	while (*args)
	{
		while (args[0][index])
		{
			if (args[0][index] == '\\')
				index = handle_backslash(args, index, true);
			else if (ft_strrchr("\"\'", args[0][index]))
				handle_quotations(&args[0], &index,
				env_list, prev_exit_status);
			else if (args[0][index] == '$')
				index = insert_variable(&args[0], index,
				env_list, prev_exit_status);
			else
				index++;
		}
		if (single_run)
			break ;
		args++;
		index = 0;
	}
}
