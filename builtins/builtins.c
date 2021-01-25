/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 19:30:41 by rprieto-          #+#    #+#             */
/*   Updated: 2021/01/25 19:16:08 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**		
*/

t_bool	export(t_list *envp, char *arg)
{
	int		i;
	t_bool	correct;

	i = 0;
	correct = false;
	//TODO: debería de checkear el argumento, puede que en otra función, mirando que mínimo tiene
	//un carácter, un igual y otro carácter
	if (ft_strlen(arg) < 3)
		return (false);
	while (arg[i])
	{
		if (*arg == '=' && *(arg + 1))
			correct = true;
		i++;
	}
	if (!correct)
		return (false);
	ft_lstadd_back(&envp, ft_lstnew(arg));
	return (true);
}

/*
**		Crea un string concatenando el nombre de la variable más un =
**		Luego recorre las variables de entorno comparandolas con el string creado
**		Si encuentra una variable de entorno que coincide, la borra
*/

void	unset(t_list *envp, char *var_name)
{
	t_list	*aux;
	t_list	*prev_aux;
	char	*var_equal;
	int		var_equal_length;

	var_equal = ft_strjoin(var_name, "=");
	var_equal_length = ft_strlen(var_equal);
	if (envp->next &&
	!ft_strncmp((char*)envp->content, var_equal, var_equal_length))
	{
		ft_lstdelone(envp, free);	
		return ;
	}
	prev_aux = envp;
	aux = envp->next;
	while (aux)
	{
		if (ft_strncmp((char*)aux->content, var_equal, ft_strlen(var_equal)))
		{
			prev_aux = aux->next;
			ft_lstdelone(aux, free);
			return ;
		}
		prev_aux = aux;
		aux = aux->next;
	}
}

t_list	*get_env_list(const char **envp)
{
	t_list	*env_list;
	t_list	*aux;

	env_list = ft_lstnew(*envp);
	aux = env_list;
	envp++;
	while (*envp)
	{
		aux->next = ft_lstnew(*envp);
		aux = aux->next;
		envp++;
	}
	return (env_list);
}
