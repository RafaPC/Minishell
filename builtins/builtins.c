/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 19:30:41 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/22 09:51:41 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Receives a string and returns false if it contains a char that is not
** alphanumeric or '_' (underscore)
*/

t_bool	valid_env_characters(char *var_name)
{
	if (ft_isdigit(*var_name))
		return (false);
	while (*var_name)
	{
		if (!ft_isalpha(*var_name) && !ft_isdigit(*var_name)
			&& *var_name != '_')
			return (false);
		var_name++;
	}
	return (true);
}

/*
** Prints all the environment variables with values
** Adds a '\n' to the end of each variable
*/

void	env(t_list *env_list, char *args)
{
	char	*content;

	if (args)
	{
		ft_printf(STDERR_FILENO,
		"env: ‘%s’: No such file or directory\n", args);
		errno = 127;
		return ;
	}
	while (env_list)
	{
		content = (char*)env_list->content;
		if (ft_strchr(content, '='))
			ft_printf(STDOUT_FILENO, "%s\n", content);
		env_list = env_list->next;
	}
}

/*
**	Prints all the arguments to standart output
*/

void	echo(char **args, int i)
{
	t_bool	newline_flag;

	newline_flag = false;
	while (*args && !ft_strncmp(*args, "-n", 2))
	{
		while ((*args)[i] && (*args)[i] == 'n')
			i++;
		if (!(*args)[i])
		{
			i = 1;
			newline_flag = true;
			args++;
		}
		else
			break ;
	}
	while (*args)
	{
		ft_putstr_fd(*args++, STDOUT_FILENO);
		if (*args)
			write(STDOUT_FILENO, " ", 1);
	}
	if (!newline_flag)
		write(STDOUT_FILENO, "\n", 1);
}

/*
** Prints the current working directory to the standart output
*/

void	pwd(void)
{
	char	*cwd;

	if ((cwd = getcwd(NULL, 0)))
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);
		write(STDOUT_FILENO, "\n", 1);
		free(cwd);
	}
}
