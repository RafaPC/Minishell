/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 20:54:28 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/16 20:21:46 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include "minishell.h"

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

void	export(t_list *envp, char *arg)
{
	t_list	*aux;

	aux = envp;
	if (arg == NULL || *arg == '\0')
	{
		export_print(envp);
		return ;
	}
	while (aux)
	{
		if (!ft_strncmp(arg, (char*)aux->content, (ft_strchr(arg, '='))
			? ft_get_index_of(arg, '=') : ft_strlen(arg)))
		{
			free(aux->content);
			aux->content = ft_strdup(arg);
			return ;
		}
		aux = aux->next;
	}
	ft_lstadd_back(&envp, ft_lstnew(ft_strdup(arg)));
}

/*
** Writes all the environment variables in the standart output fd
*/

void	export_print(t_list *envp)
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
}
