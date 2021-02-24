/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executables_paths.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 23:35:42 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/24 16:19:43 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include "libft.h"
#include "minishell.h"

/*
** PATHS
** Array of strings of paths to executables
** COMMAND
** String of the command to search
*/

char	*get_command_path(char **paths, char *command)
{
	int		i;
	int		fd;
	char	*slash_command;
	char	*command_path;

	i = 0;
	slash_command = ft_strjoin("/", command);
	while (paths[i])
	{
		command_path = ft_strjoin(paths[i], slash_command);
		if ((fd = open(command_path, O_RDONLY)) != -1)
		{
			close(fd);
			free(slash_command);
			ft_array_clear((void**)paths, free);
			return (command_path);
		}
		free(command_path);
		i++; 
	}
	free(slash_command);
	ft_array_clear((void**)paths, free);
	return (NULL);
}

/*
** Returns a strings array of the $PATH variable paths
*/

char	**get_path(t_list *envp)
{
	char	*path_value;
	char	**path_splitted;

	path_splitted = NULL;
	path_value = get_env_var("PATH", envp);
	if (path_value)
	{
		path_splitted = ft_split(path_value, ':');
		free(path_value);
	}
	return (path_splitted);
}
