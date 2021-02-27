/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 19:30:41 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/27 20:05:20 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Prints all the environment variables with values
** Adds a '\n' to the end of each variable
** TODO: checkear
*/

t_bool	env(t_list *envp, char **args)
{
	char	*check;
	char	*content;

	if (*args)
	{
		ft_printf("env: %s: No such file or directory\n");
		return (true);
	}
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
	return (true);
}

/*
** Called when "exit" is written in the shell
** Frees the t_list* list of environment variables elements
*/

void	ft_exit(t_command *commands, t_list **env_list, char **env_array)
{
	free_commands(commands);
	ft_lstclear(env_list, free);
	free(env_array);
	exit(0);
}

/*
**	Prints all the arguments to standart output
*/

t_bool	echo(char **args)
{
	t_bool	newline_flag;
	int		i;

	i = 1;
	newline_flag = false;
	if (*args && !ft_strncmp(*args, "-n", 2))
	{
		while ((*args)[i] && (*args)[i] == 'n')
			i++;
		if (!(*args)[i])
		{
			newline_flag = true;
			args++;
		}
	}
	while (*args)
	{
		ft_putstr_fd(*args++, STDOUT_FILENO);
		if (*args)
			write(STDOUT_FILENO, " ", 1);
	}
	if (!newline_flag)
		write(STDOUT_FILENO, "\n", 1);
	return (true);
}

/*
** Prints the current working directory to the standart output
*/

t_bool	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	ft_putstr_fd(cwd, STDOUT_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	free(cwd);
	return (true);
}

/*
**		Changes the current working directory
*/

t_bool	cd(char **args)
{
	if (args[0] && args[1])
		ft_putstr_fd("minishell: cd: too many arguments\n", STDOUT_FILENO);
	else if (args[0])
	{
		if (chdir(args[0]) == -1)
		{
			//TODO: coger el errno y eso
			return (false);
		}
	}
	return (true);
}
