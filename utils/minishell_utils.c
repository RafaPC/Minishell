/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/07 12:23:34 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/22 11:46:56 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			var_value = ft_strdup((char *)(content + var_length));
		env_list = env_list->next;
	}
	if (!var_value && !ft_strncmp("PWD", var, 4))
		var_value = getcwd(NULL, 0);
	free(var_equal);
	return (var_value);
}
