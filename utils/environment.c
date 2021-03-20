/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 19:05:22 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/20 13:54:15 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** This function is called if the program doesn't receive any environment var
** It adds the basic variables to the environment var list
*/

t_list	*create_new_env_list(char *command_path)
{
	t_list	*env;
	char	*aux;

	aux = getcwd(NULL, 0);
	env = ft_lstnew(ft_strjoin("PWD=", aux));
	free(aux);
	ft_lstadd_back(&env, ft_lstnew(ft_strdup("SHLVL=1")));
	aux = ft_strdup(command_path);
	ft_lstadd_back(&env, ft_lstnew(ft_strjoin("_=", aux)));
	free(aux);
	return (env);
}

/*
** Checks if the t_list of environment variables has the basic variables
** If the variables doesn't exist they are added to the list
*/

t_list	*check_and_fix_env(t_list *env, char *command_path)
{
	char	*aux;

	if ((aux = get_env_var("PWD", env)))
		free(aux);
	else
	{
		aux = getcwd(NULL, 0);
		ft_lstadd_back(&env, ft_lstnew(ft_strjoin("PWD=", aux)));
		free(aux);
	}
	if ((aux = get_env_var("_", env)))
		free(aux);
	else
		ft_lstadd_back(&env, ft_lstnew(ft_strjoin("_=", command_path)));
	if ((aux = get_env_var("SHLVL", env)))
		free(aux);
	else
		export_variable(&env, "SHLVL=1");
	return (env);
}

/*
** Parameter:
** env -> array of environment variables strings
**
** Creates a list of t_list* elements with the
** environment variables strings as content
**
** Returns a t_list pointer to the first element of the list
*/

t_list	*create_env_list(const char **env, char *command_path)
{
	t_list	*env_list;
	t_list	*aux;
	char	*str_aux;

	if (!env || !(*env))
		return (create_new_env_list(command_path));
	env_list = ft_lstnew(ft_strdup(*env));
	env++;
	aux = env_list;
	while (*env)
	{
		if (!ft_strncmp(*env, "SHLVL=", 6))
		{
			str_aux = ft_itoa(ft_atoi(&(*env)[6]) + 1);
			aux->next = ft_lstnew(ft_strjoin("SHLVL=", str_aux));
			free(str_aux);
		}
		else
			aux->next = ft_lstnew(ft_strdup(*env));
		aux = aux->next;
		env++;
	}
	return (check_and_fix_env(env_list, command_path));
}
