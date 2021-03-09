/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/07 12:23:34 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/27 11:25:49 by aiglesia         ###   ########.fr       */
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

t_list	*create_env_list(const char **env)
{
	t_list	*env_list;
	t_list	*aux;

	if (!env || !(*env))
		return (NULL);
	env_list = ft_lstnew(ft_strdup(*env));
	aux = env_list;
	env++;
	while (*env)
	{
		aux->next = ft_lstnew(ft_strdup(*env));
		aux = aux->next;
		env++;
	}
	return (env_list);
}

/*
** Transforms the t_list* list of environment variables elements to
** a double pointer of chars so that it can be used as the third argument in
** execve()
*/

char	**env_list_to_array(t_list *env_list)
{
	char	**env_array;
	char	*content;
	int		i;

	i = 0;
	env_array = (char**)ft_calloc(
	(ft_lstsize(env_list) + 1), sizeof(char*));
	while (env_list)
	{
		content = (char*)env_list->content;
		env_array[i] = content;
		env_list = env_list->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

/*
** Parameters:
** var -> environment variable name to search in the list
** envp -> list of environment variables
**
** Searches in the environment variables list for the var name
** and if it is found returns its value
*/

char	*get_env_var(char *var, t_list *env_list)
{
	char	*content;
	char	*var_equal;
	int		var_length;
	char	*var_value;

	var_value = NULL;
	var_equal = ft_strjoin(var, "=");
	var_length = ft_strlen(var_equal);
	while (env_list && !var_value)
	{
		content = (char*)(env_list->content);
		if (var_length == (ft_get_index_of(content, '=') + 1) &&
		!ft_strncmp(content, var_equal, var_length))
			var_value =  (char *)(content + var_length);
		env_list = env_list->next;
	}
	if (!var_value && !ft_strncmp("PWD", var, 4))//FIXME: maybe lo mismo para OLDPWD
		var_value = getcwd(NULL, 0);
	free(var_equal);
	return (var_value);
}

/*
**	FIXME: ESTO SOLO TIENE FINES DEBUGILES
*/

char	**get_false_env_array(void)
{
	char	**env_list;

	env_list = malloc(sizeof(char**) * 6);
	env_list[0] = ft_strdup("USER=user42");
	env_list[1] = ft_strdup("PATH=/home/user42/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin");
	env_list[2] = ft_strdup("PWD=/home/user42/Desktop/minishell");
	env_list[3] = ft_strdup("LANGUAGE=en");
	env_list[4] = ft_strdup("HOME=/home/user42");
	env_list[5] = NULL;
	return (env_list);
}
