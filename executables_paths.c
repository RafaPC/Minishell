/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executables_paths.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 23:35:42 by rprieto-          #+#    #+#             */
/*   Updated: 2021/01/25 19:13:18 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int		get_exec_paths_legnth(char **exec_paths)
{
	int i;

	i = 0;
	while (exec_paths[i])
		i++;
	return (i);
}

char	**get_command_directories(char **exec_paths, char *command)
{
	int		i;
	char	*slash_command;
	char	**command_paths;

	i = 0;
	command_paths = (char**)malloc(
		sizeof(char*) * (get_exec_paths_legnth(exec_paths) + 1));
	slash_command = ft_strjoin("/", command);
	while (exec_paths[i])
	{
		command_paths[i] = ft_strjoin(exec_paths[i], slash_command);
		i++;
	}
	command_paths[i] = NULL;
	free(slash_command);
	return (command_paths);
}

char	**get_path(t_list *envp)
{
	t_list	*aux;
	char	*content;

	if (!envp)
		return (NULL);
	aux = envp;
	while (aux)
	{
		content = (char*)(aux->content);
		if (!ft_strncmp(content, "PATH=", 5))
			return (ft_split(content + 5, ':'));
		aux = aux->next;
	}
	return (NULL);
}