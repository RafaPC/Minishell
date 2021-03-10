/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 14:39:56 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/28 11:17:55 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include "minishell.h"


t_bool	unset_variable(t_list **env, char *arg)
{
	t_list	*aux;
	int		compare_length;
	int		compare_result;

	compare_length = ft_strlen(arg) + 1;
	compare_result = ft_strncmp(
		(char*)(*env)->content, arg, compare_length);
	if (compare_result == 0 || compare_result == 61)
	{
		aux = *env;
		*env = aux->next;
		ft_lstdelone(aux, free);
		return (true);
	}
	else if ((env[0])->next)
		return (unset_recursive((env[0])->next, *env, arg, compare_length));
	else
		return (false);
}

/*
** Compares each variable from the list with the name given as argument
** It compares n + 1 characters, n being the name argument length
** When ft_strncmp() compares 2 different characters, it returns the difference
** For example: return (string1[i] - string2[i])
**
** So there are 3 possible cases:
** There's a variable 'NAME=value\0' and the function receives 'NAME\0'
** When comparing them, it'll reach the 5th char and will return '=' - '\0'
** which is equal to 61
** '=' having 61 value in the ASCII table and '\0' being 0
** If the variable is stored 'NAME\0', ft_strncmp() won't see any
** difference and will return 0
** In any other case it will return other number product of the substraction
**
** It first checks if the first variable is the one searched, if it is then
** sets the t_list variable accordingly wether there are more elements or not
** If it doesn't match the first element then calls a function that iterates
** recursively over the list and deletes a variable if it matches the name
** TODO: checkear si podría abstraer esto para que el primer caso
** fuera igual que otro cualquiera
*/

int	unset(t_list **env, char **args)
{
	t_bool	aux;
	t_bool	env_changed;

	env_changed = false;
	while (*args)
	{
		if (!valid_env_characters(*args) || ft_strchr(*args, '='))
			ft_printf(STDERR_FILENO,
			"unset: '%s': not a valid identifier\n", *args);//FIXME: checkear esto
		else if (*env)
		{
			aux = unset_variable(env, *args);
			if (!env_changed)
				env_changed = aux;
		}
		args++;
	}
	return (env_changed ? 2 : 1);
}

/*
** Parameters:
** envp -> t_list element used to iterate and compare its content with var_name
** previous_aux -> t_list element used to connect the list when envp is deleted
** var_name -> name of the variable to delete
** compare_length -> number of chars to compare equal to strlen(var_name) + 1
*/

t_bool	unset_recursive(t_list *env_list, t_list *previous_aux,
char *var_name, int compare_length)
{
	int		compare_result;

	compare_result = ft_strncmp(
		(char*)env_list->content, var_name, compare_length);
	if (compare_result == 0 || compare_result == 61)
	{
		previous_aux->next = env_list->next;
		ft_lstdelone(env_list, free);
		return (true);
	}
	else if (env_list->next)
		return (unset_recursive(
			env_list->next, env_list, var_name, compare_length));
	else
		return (false);
}
