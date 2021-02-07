/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 19:30:41 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/07 13:45:28 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Concatenates all the environment variables into a string
** It only concatenates the variables with value
** Adds a '\n' to the end of each variable
*/

char	*env(t_list *envp, t_list *envp_copy, int length, int i)
{
	char	*check;
	char	*content;
	char	*return_string;

	while (envp)
	{
		check = ft_strchr((char*)envp->content, '=');
		if (check != NULL && *(check + 1) != '\0')
			length += ft_strlen((char*)envp->content) + 1;
		envp = envp->next;
	}
	return_string = (char*)ft_calloc((length + 1), sizeof(char));
	while (envp_copy)
	{
		content = (char*)envp_copy->content;
		check = ft_strchr(content, '=');
		if (check != NULL && *(check + 1) != '\0')
		{
			while (*content)
				return_string[i++] = *(content++);
			return_string[i++] = '\n';
		}
		envp_copy = envp_copy->next;
	}
	return (return_string);
}

/*
** Called when "exit" is written in the shell
** Frees the t_list* list of environment variables elements
*/

void	ft_exit(t_list **envp)
{
	ft_lstclear(envp, free);
	exit(0);
}
