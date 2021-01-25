/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 01:52:02 by rprieto-          #+#    #+#             */
/*   Updated: 2021/01/25 19:12:59 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include "libft.h"
#include "minishell.h"

int		main(int argc, char const **argv, char const **envp)
{
	char	*buffer;
	t_list	*envp_list;

	buffer = (char*)malloc(sizeof(char) * 1025);
	envp_list = get_env_list(envp);
	while (true)
	{
		read_input(buffer);
		tabs_to_spaces(buffer);
		buffer = ft_strtrim(buffer, " ");
		printf("El texto introducido es: %s\n", buffer);
		if (execute_command(get_path(envp_list), buffer))
		{
			printf("Command %s exists\n", buffer);
		}
		else
			printf("Command %s doesn't exist\n", buffer);
		empty_buffer(buffer);
	}
	return (0);
}

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
