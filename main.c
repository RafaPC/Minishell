/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 01:52:02 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/07 13:53:03 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include "minishell.h"

int		main(int argc, char const **argv, char const **envp)
{
	char	*buffer;
	char	*aux;
	t_list	*envp_list;

	buffer = (char*)malloc(sizeof(char) * 1025);
	envp_list = create_env_list(envp);
	while (true)
	{
		read_input(buffer);
		tabs_to_spaces(buffer);
		aux = buffer;
		buffer = ft_strtrim(buffer, " ");
		free(aux);
		char **command_and_args = ft_split(buffer, ' ');
		//Aquí un if por cada builtin (echo, export, unset, cd, pwd...)
		if (!ft_strncmp(command_and_args[0], "export", 6))
		{
			char *result = export(envp_list, command_and_args[1]);
			if (result)
			{
				ft_putstr_fd(result, 1);
				free(result);
			}
		}
		else if (!ft_strncmp(command_and_args[0], "unset", 5))
			unset(&envp_list, command_and_args[1]);
		else if (!ft_strncmp(command_and_args[0], "env", 3))
		{
			char *env_string = env(envp_list, envp_list, 0, 0);
			if (env_string)
			{
				ft_putstr_fd(env_string, 1);
				free(env_string);
			}
		}
		else if (!ft_strncmp(command_and_args[0], "exit", 4))
			ft_exit(&envp_list);
		else if (execute_command(get_path(envp_list), command_and_args[0]))
		{
			printf("Command %s exists\n", buffer);
		}
		else
			printf("Command %s doesn't exist\n", buffer);
		//Libera memoria
		for (int i = 0; command_and_args[i]; i++)
		{
			free(command_and_args[i]);
			if (!command_and_args[i + 1])
				free(command_and_args[i + 1]);
		}
		free(command_and_args);
		empty_buffer(buffer);
	}
	return (0);
}
