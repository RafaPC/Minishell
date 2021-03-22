/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 20:54:28 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/22 09:58:54 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Prints an error message to standard error and set errno to EPERM if
** the token is an empty string
** If the token is a variable without value and the variable al ready exists,
** it does nothing
*/

t_bool	check_export_error(char *identifier, t_list *env_list)
{
	char	*aux;

	if (*identifier == '\0')
	{
		ft_printf(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier\n", identifier);
		errno = EPERM;
		return (true);
	}
	else if (!ft_strchr(identifier, '=') &&
		(aux = get_env_var(identifier, env_list)))
	{
		free(aux);
		return (true);
	}
	return (false);
}

/*
** If the argument is null it writes all the variables
** formatted as -> declare -x "NAME=VALUE"
** If it's not null, searches in the list for a variable with the arg name
** and when finds it sets its content with the arg string
** If no variable with the arg name is found, a new element is added
** to the list with arg as its content
*/

void	export(t_list **env_list, char **args, int equal_position)
{
	char	*str_aux;

	if (*args == NULL)
		export_print(*env_list);
	while (*args)
	{
		if (!check_export_error(*args, *env_list))
		{
			equal_position = ft_get_index_of(*args, '=');
			str_aux = (equal_position != -1)
			? ft_strncpy(*args, equal_position) : ft_strdup(*args);
			if (!str_aux)
				return ;
			if (**args == '=' || !valid_env_characters(str_aux))
			{
				ft_printf(STDERR_FILENO,
					"minishell: export: `%s': not a valid identifier\n", *args);
				errno = EPERM;
			}
			else if (!export_variable(env_list, *args))
				return ;
			free(str_aux);
		}
		args++;
	}
}

/*
** Receives an environment list and an argument and searches if the argument
** already exists in the list.
** If it exists it'll modify its value with the new one
** If it doesn't, it will add it to the end of the list
*/

t_bool	export_variable(t_list **env_list, char *arg)
{
	t_list	*aux;
	int		arg_length;
	int		env_length;

	aux = *env_list;
	while (aux)
	{
		arg_length = (ft_strchr(arg, '=')
		? ft_get_index_of(arg, '=') : (int)ft_strlen(arg));
		env_length = (ft_strchr(aux->content, '=')
		? ft_get_index_of(aux->content, '=') : (int)ft_strlen(aux->content));
		if ((arg_length == env_length) && !ft_strncmp(arg, (char*)aux->content,
		arg_length > env_length ? arg_length : env_length))
		{
			free(aux->content);
			if (!(aux->content = ft_strdup(arg)))
				return (false);
			return (true);
		}
		aux = aux->next;
	}
	if (!(aux = ft_lstnew(ft_strdup(arg))))
		return (false);
	ft_lstadd_back(env_list, aux);
	return (true);
}

/*
** Writes all the environment variables in the standart output
*/

void	export_print(t_list *env_list)
{
	char	*content;

	while (env_list)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		content = (char*)env_list->content;
		while (*content && *content != '=')
			write(STDOUT_FILENO, content++, 1);
		if (*content == '=')
			ft_printf(STDOUT_FILENO, "=\"%s\"", ++content);
		write(STDOUT_FILENO, "\n", 1);
		env_list = env_list->next;
	}
}
