/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 14:39:56 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/20 13:33:23 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** The function iterates over the arguments given
** If an argument has an invalid character a message is printed
** If the argument is correct it calls unset_variable()
*/

void	unset(t_list **env, char **args)
{
	while (*args)
	{
		if (!valid_env_characters(*args) || ft_strchr(*args, '='))
			ft_printf(STDERR_FILENO,
			"unset: `%s': not a valid identifier\n", *args);
		else if (*env)
			unset_variable(env, *args);
		args++;
	}
}

void	unset_variable(t_list **env, char *arg)
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
		return ;
	}
	else if ((env[0])->next)
		unset_recursive((env[0])->next, *env, arg, compare_length);
}

/*
** Parameters:
** env_list -> element used to iterate and compare its content with var_name
** previous_aux -> element used to connect the list when envp is deleted
** var_name -> name of the variable to delete
** compare_length -> number of chars to compare equal to strlen(var_name) + 1
*/

void	unset_recursive(t_list *env_list, t_list *previous_aux,
char *var_name, int compare_length)
{
	int	compare_result;

	compare_result = ft_strncmp(
		(char*)env_list->content, var_name, compare_length);
	if (compare_result == 0 || compare_result == 61)
	{
		previous_aux->next = env_list->next;
		ft_lstdelone(env_list, free);
		return ;
	}
	else if (env_list->next)
		unset_recursive(env_list->next, env_list, var_name, compare_length);
}
