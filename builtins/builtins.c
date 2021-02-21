/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 19:30:41 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/16 21:00:10 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Prints all the environment variables with values
** Adds a '\n' to the end of each variable
*/

void	env(t_list *envp)
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

/*
**	Prints all the arguments to standart output
*/

void	echo(char **args)
{
	t_bool	newline_flag;
	int		i;

	i = 0;
	newline_flag = false;
	if (!ft_strncmp(*args, "-n", 2))
	{
		while ((*args)[i] && (*args)[i] == 'n')
			i++;
		if (!(*args)[i])
		{
			newline_flag = true;
			args++;
		}
			
	}
	while (args)
	{
		ft_putstr_fd(*args++, STDOUT_FILENO);
		write(STDOUT_FILENO, " ", 1);
	}
	if (newline_flag)
		write(STDOUT_FILENO, "\n", 1);
}
