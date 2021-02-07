/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executables_paths.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 23:35:42 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/07 13:58:27 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include "libft.h"
#include "minishell.h"

/*
** Parameters:
** exec_paths -> array of strings of executables paths
** buffer -> the arguments, ESTO TIENE QUE CAMBIAR //TODO:
**
** Calls a function to concatenate the paths with the command name
** Then tries to open every file to check if the command exists in these paths
*/

int		execute_command(char **exec_paths, char *buffer)
{
	char		**exec_paths_list;
	int			i;
	char		*command;
	int			fd;

	i = 0;
	command = (char*)malloc(sizeof(char) * (ft_strlen(buffer) + 1));
	ft_strlcpy(command, buffer, ft_strlen(buffer) + 1);
	exec_paths_list = get_command_directories(exec_paths, command);
	while (exec_paths_list[i])
	{
		fd = open(exec_paths_list[i], O_RDONLY);
		if (fd != -1)
		{
			close(fd);
			printf("Path to executable %s is %s\n", command, exec_paths_list[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

/*
** Parameters:
** exec_paths -> array of strings with executables paths
** command -> command name string
**
** Creates a strings array of concated executables directories + command name
*/

char	**get_command_directories(char **exec_paths, char *command)
{
	int		i;
	char	*slash_command;
	char	**command_paths;

	i = 0;
	command_paths = (char**)ft_calloc(
		(ft_array_size((void**)exec_paths) + 1), sizeof(char*));
	slash_command = ft_strjoin("/", command);
	while (exec_paths[i])
	{
		command_paths[i] = ft_strjoin(exec_paths[i], slash_command);
		i++;
	}
	free(slash_command);
	return (command_paths);
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
