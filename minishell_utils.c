/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/07 12:23:34 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/07 13:46:56 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Parameter:
** envp -> array of environment variables strings
**
** Creates a list of t_list* elements with the
** environment variables strings as content
**
** Returns a t_list pointer to the first element of the list
*/

#include "libft.h"

t_list	*create_env_list(const char **envp)
{
	t_list	*env_list;
	t_list	*aux;

	env_list = ft_lstnew(ft_strdup(*envp));
	aux = env_list;
	envp++;
	while (*envp)
	{
		aux->next = ft_lstnew(ft_strdup(*envp));
		aux = aux->next;
		envp++;
	}
	return (env_list);
}

/*
** Transforms the t_list* list of environment variables elements to
** a double pointer of chars so that it can be used as the third argument in
** execve()
*/

char	**env_list_to_char(t_list *envp)
{
	char	**env_char_list;
	char	*content;
	int		i;

	i = 0;
	env_char_list = (char**)ft_calloc(
	(ft_lstsize(envp) + 1), sizeof(char*));
	while (envp)
	{
		content = (char*)envp->content;
		env_char_list[i] = content;
		envp = envp->next;
		i++;
	}
	env_char_list[i] = NULL;
	return (env_char_list);
}

/*
** Parameters:
** var -> environment variable name to search in the list
** envp -> list of environment variables
**
** Searches in the environment variables list for the var name
** and if it is found returns its value
*/

char	*get_env_var(char *var, t_list *envp)
{
	char	*content;
	char	*var_equal;
	int		var_length;

	var_equal = ft_strjoin(var, "=");
	var_length = ft_strlen(var_equal);
	while (envp)
	{
		content = (char*)(envp->content);
		if (!ft_strncmp(content, var_equal, var_length))
		{
			free(var_equal);
			return (ft_strdup(content + var_length));
		}
		envp = envp->next;
	}
	free(var_equal);
	return (NULL);
}
