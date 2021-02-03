/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_insertions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 11:41:44 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/03 12:16:18 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** INPUT = Parse input.
**
** I = Index to the current parsing location.
**
** Same as the double quote function, with the exception that no conversions are done.
*/

int	handle_single_quotations(char **input, int i)
{
	i = ft_extract(input, i, 1);
	while ((*input)[i])
	{
		if ((*input)[i] == '\'')
			return (ft_extract(input, i, 1));
		i++;
	}
	return (i);
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
** Might consider stopping the parsing and printing an error if no closing quotation is found. 
*/

int	handle_double_quotations(char **input, int i)
{
	i = ft_extract(input, i, 1);
	while ((*input)[i])
	{
		if ((*input)[i] == '\\')
		{
			if (ft_strchr("$`\"", (*input)[i + 1]))
				i = ft_extract(input, i, 1) + 1;
			else
				i++;
		}
		if ((*input)[i] == '$')
			i = insert_variable(input, i);
		if ((*input)[i] == '\"')
			return (ft_extract(input, i, 1));
		else
			i++;
	}
	return (i);
}

/*
** INPUT = Parse input.
**
** I = Index to the current parsing location.
**
** Buffer function as an intermediary to call the respective function.
** Consider removal if enough space in the father function.
*/

int	handle_quotations(char **input, int i)
{
	if ((*input)[i] == '\"')
		return (handle_double_quotations(input, i));
	else if ((*input)[i] == '\'')
		return (handle_single_quotations(input, i));
	return (i);
}

/*
**INPUT = Parse input.
**
** Index = Index to the '$'.
**
** Called when a '$' is found. It removes said '$' and any alphanumeric and '_' characters that may preceed it.
** Said characters are saved to a temporal string to search for any stored variable that may match it.
** If one is found, the value of said variable is inserted.
** In any case, an index is returned pointing to the char preceeding the insertion (or lack thereof);
*/

int	insert_variable(char **input, int index)
{
	int		j;
	char	*aux;
	char	*variable;

	j = 1;
	while (ft_isalnum((*input)[index + j]) || (*input)[index + j] == '_')
		j++;
	aux = ft_strncat_in(0, &(*input)[index + 1], j - 1);
	variable = ft_strdup("listen"); //remove
	//variable = get_variable(); // Returns a string with the contents of the variable. If none is found, it returns an empty string.
	index = ft_extract(input, index + j - 1, j);
	if (variable)
	{
		index = ft_insert(input, variable, index, ft_strlen(variable));
		free(variable);
	}
	free(aux);
	return (index);
}
