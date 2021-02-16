/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 01:52:02 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/16 19:25:44 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include "minishell.h"
#include <signal.h>

void	signal_interrump(int signal)
{
	ft_putstr_fd("\nPrompt:", 1);
}

int		main(int argc, char const **argv, char const **envp)
{
	char	*buffer;
	char	*aux; 
	t_list	*envp_list;


	buffer = (char*)malloc(sizeof(char) * 1025);
	envp_list = create_env_list(envp);
	signal(SIGINT, signal_interrump);
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
			export(envp_list, command_and_args[1]);
		else if (!ft_strncmp(command_and_args[0], "unset", 5))
			unset(&envp_list, command_and_args[1]);
		else if (!ft_strncmp(command_and_args[0], "env", 3))
			env(envp_list, 0, 0);
		else if (!ft_strncmp(command_and_args[0], "exit", 4))
			ft_exit(&envp_list);
		else if (execute_command(get_path(envp_list), command_and_args[0]))
		{
			printf("Command %s exists\n", buffer);
		}
		else
			printf("Command %s doesn't exist\n", buffer);
		//Libera memoria
		ft_array_clear((void**)command_and_args, free);
		empty_buffer(buffer);
	}
	return (0);
}
