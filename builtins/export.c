/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 20:54:28 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/28 11:20:50 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include "minishell.h"
#include "ft_printf.h"

/*
** Receives a string and returns false if it contains a char that is not
** alphanumeric or '_' (underscore)
*/

t_bool	valid_env_characters(char *var_name)
{
	if (ft_isdigit(*var_name++))
		return (false);
	while (*var_name)
	{
		if (!ft_isalpha(*var_name) && !ft_isdigit(*var_name)
			&& *var_name != '_')
			return (false);
		var_name++;
	}
	return (true);
}

/*
** Receives an environment list and an argument and searches if the argument
** already exists in the list.
** If it exists it'll modify its value with the new one
** If it doesn't, it will add to the end of the list
*/

t_bool	export_variable(t_list **env_list, char *arg)
{
	t_list	*aux;

	aux = *env_list;
	while (aux)
	{
		if (!ft_strncmp(arg, (char*)aux->content, (ft_strchr(arg, '='))
			? ft_get_index_of(arg, '=') : (int)ft_strlen(arg)))
		{
			free(aux->content);
			if ((aux->content = ft_strdup(arg)) == NULL) //ERROR de malloc
				return (0);
			return (2);
		}
		aux = aux->next;
	}
	if ((aux = ft_lstnew(ft_strdup(arg))) == NULL) //ERROR de malloc
		return (false);
	ft_lstadd_back(env_list, aux);
	return (true);
}

/*
** Parameters:
** arg -> string that defines a variable name or name=value
**
** If the argument is null it writes all the variables
** formatted as -> declare -x "NAME=VALUE"
** If it's not null, searches in the list for a variable with the arg name
** and when finds it sets its content with the arg string
** If no variable with the arg name is found, a new element is added
** to the list with arg as its content
*/

int		export(t_list **env, char **args)
{
	char	*str_aux;

	if (*args == NULL || **args == '\0')
		return (export_print(*env));
	while (*args)
	{
		str_aux = ((ft_strchr(*args, '='))
		? ft_strncpy(*args, ft_get_index_of(*args, '=') - 1)
		: ft_strdup(*args));
		if (**args == '=' || !valid_env_characters(str_aux))
			ft_printf("minishell: export: `%s': not a valid specifier\n",
				*args);
		else if (!export_variable(env, *args))
		{
			ft_printf("Error al alocar memoria\n"); // ERROR
			return (0);
		}
		free(str_aux);
		args++;
	}
	return (2);
}

/*
** Writes all the environment variables in the standart output
*/

int		export_print(t_list *envp)
{
	char	*content;

	while (envp)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		content = (char*)envp->content;
		while (*content && *content != '=')
			write(STDOUT_FILENO, content++, 1);
		if (*content == '=')
		{
			write(STDOUT_FILENO, content++, 1);
			write(STDOUT_FILENO, "\"", 1);
			while (*content)
				write(STDOUT_FILENO, content++, 1);
			write(STDOUT_FILENO, "\"", 1);
		}
		write(STDOUT_FILENO, "\n", 1);
		envp = envp->next;
	}
	return (1);
}
