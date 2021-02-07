/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 14:39:56 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/06 12:31:10 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include "minishell.h"

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
*/

void	unset(t_list **envp, char *var_name)
{
	t_list	*aux;
	int		compare_length;
	int		compare_result;

	if (ft_strchr(var_name, '='))
		printf("unset: '%s': not a valid identifier\n", var_name);
	else if (*envp)
	{
		compare_length = ft_strlen(var_name) + 1;
		compare_result = ft_strncmp(
			(char*)(*envp)->content, var_name, compare_length);
		if (compare_result == 0 || compare_result == 61)
		{
			aux = *envp;
			*envp = NULL;
			ft_lstdelone(aux, free);
		}
		else if ((*envp)->next)
			unset_recursive((*envp)->next, *envp, var_name, compare_length);
	}
}

/*
** Parameters:
** envp -> t_list element used to iterate and compare its content with var_name
** previous_aux -> t_list element used to connect the list when envp is deleted
** var_name -> name of the variable to delete
** compare_length -> number of chars to compare equal to strlen(var_name) + 1
*/

void	unset_recursive(t_list *envp, t_list *previous_aux,
char *var_name, int compare_length)
{
	int		compare_result;

	compare_result = ft_strncmp(
		(char*)envp->content, var_name, compare_length);
	if (compare_result == 0 || compare_result == 61)
	{
		previous_aux->next = envp->next;
		ft_lstdelone(envp, free);
	}
	else if (envp->next)
		unset_recursive(envp->next, envp, var_name, compare_length);
}
