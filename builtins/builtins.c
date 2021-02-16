/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 19:30:41 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/16 20:14:50 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Prints all the environment variables with values
** Adds a '\n' to the end of each variable
*/

void	env(t_list *envp, int length, int i)
{
	char	*check;
	char	*content;

	while (envp)
	{
		content = (char*)envp->content;
		if (ft_strchr(content, '='))
		{
			ft_putstr_fd(content, STDOUT_FILENO);
			write(STDOUT_FILENO, "\n", 1);
		}
		envp = envp->next;
	}
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
