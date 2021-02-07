/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 20:54:28 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/07 13:39:36 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include "minishell.h"

/*
** Parameters:
** arg -> string that defines a variable name or name=value
**
** If the argument is null it returns all the variables concatenated
** in one string formatted as -> declare -x "NAME=VALUE"
** If it's not null, searches in the list for a variable with the arg name
** and when finds it sets its content with the arg string
** If no variable with the arg name is found, a new element is added
** to the list with arg as its content
*/

char	*export(t_list *envp, char *arg)
{
	t_list	*aux;
	char	*return_string;

	aux = envp;
	if (arg == NULL || *arg == '\0')
	{
		if (!(return_string = get_env_string_pointer(envp)))
			return (NULL);
		return (export_print(envp, return_string));
	}
	while (aux)
	{
		if (!ft_strncmp(arg, (char*)aux->content, (ft_strchr(arg, '='))
			? ft_get_index_of(arg, '=') : ft_strlen(arg)))
		{
			free(aux->content);
			aux->content = ft_strdup(arg);
			return (NULL);
		}
		aux = aux->next;
	}
	ft_lstadd_back(&envp, ft_lstnew(ft_strdup(arg)));
	return (NULL);
}

/*
** Writes "declare -x " into the string and returns the iterator integer
*/

int		save_declarex(char **string, int i)
{
	(*string)[i++] = 'd';
	(*string)[i++] = 'e';
	(*string)[i++] = 'c';
	(*string)[i++] = 'l';
	(*string)[i++] = 'a';
	(*string)[i++] = 'r';
	(*string)[i++] = 'e';
	(*string)[i++] = ' ';
	(*string)[i++] = '-';
	(*string)[i++] = 'x';
	(*string)[i++] = ' ';
	return (i);
}

/*
** Concatenates all the environment variables into one string
*/

char	*export_print(t_list *envp, char *string)
{
	char	*elem_content;
	int		i;

	i = 0;
	while (envp)
	{
		i = save_declarex(&string, i);
		elem_content = (char*)envp->content;
		while (*elem_content && *elem_content != '=')
			string[i++] = *(elem_content++);
		if (*elem_content == '=')
		{
			string[i++] = *(elem_content++);
			string[i++] = '"';
			while (*elem_content)
				string[i++] = *(elem_content++);
			string[i++] = '"';
		}
		string[i++] = '\n';
		envp = envp->next;
	}
	return (string);
}

/*
** Calculates the length required to allocate the whole environment variables
** list into a string
** It must write "declare -x " before each line, and a "\n" after each one
** That's why it initialices the length to the number of variables multipled by
** 12, which are the number of characters of "declare -x " and "\n"
** Then for each variable, it adds the length of each string and 2 more if the
** variable has an equal sign and no quotes, so that every value is wrapped in
** quotes (double quotes by default)
*/

char	*get_env_string_pointer(t_list *envp)
{
	int		length;
	char	*content;

	length = ft_lstsize(envp) * 12;
	while (envp)
	{
		content = (char*)envp->content;
		length += ft_strlen(content);
		if (ft_strchr(content, '='))
			length += 2;
		envp = envp->next;
	}
	return ((char*)ft_calloc(length + 1, sizeof(char)));
}
