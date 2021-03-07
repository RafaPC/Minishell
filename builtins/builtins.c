/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 19:30:41 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/07 10:50:57 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Prints all the environment variables with values
** Adds a '\n' to the end of each variable
** TODO: checkear
*/

t_bool	env(t_list *env_list, char **args)
{
	char	*content;

	if (*args)
	{
		ft_printf("env: %s: No such file or directory\n");
		return (true);
	}
	while (env_list)
	{
		content = (char*)env_list->content;
		if (ft_strchr(content, '='))
		{
			ft_putstr_fd(content, STDOUT_FILENO);
			write(STDOUT_FILENO, "\n", 1);
		}
		env_list = env_list->next;
	}
	return (true);
}

/*
** Called when "exit" is written in the shell
** Frees the t_list* list of environment variables elements
** Returns the value passed as an argument.
** if more arguments are given, a 2 is returned (for some reason);
*/

void	ft_exit(t_command *commands, t_list **env_list, char **env_array)
{
	int exit_code;

	exit_code = 0;
	if (commands->tokens[1])
	{
		if (commands->tokens[2])
		{
			exit_code = 1;
			ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		}
		else if (ft_str_checkset(commands->tokens[1], "0123456789"))
			exit_code = ft_atoi(commands->tokens[1]);
		else
		{
			exit_code = 2;
			ft_putstr_fd("minishell: exit: notanumber: numeric argument required\n", STDERR_FILENO);
		}
	}
	free_commands(commands);
	ft_lstclear(env_list, free);
	free(env_array);
	exit(exit_code);
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

t_bool	cd(t_list **env_list, char **args)
{
	char	*pwd_new;
	char	*aux;

	if (args[0] && args[1])
		ft_putstr_fd("minishell: cd: too many arguments\n", STDOUT_FILENO);
	else if (args[0])
	{
		if (chdir(args[0]) == -1)
			return (false);
		if (get_env_var("OLDPWD", *env_list))
		{
			aux = get_env_var("PWD", *env_list);
			pwd_new = ft_strjoin("OLDPWD=", aux);
			export_variable(env_list, pwd_new);
			free(pwd_new);
		}
		if (get_env_var("PWD", *env_list))
		{
			aux = getcwd(NULL, 0);
			pwd_new = ft_strjoin("PWD=", aux);
			export_variable(env_list, pwd_new);
			free(aux);
			free(pwd_new);
		}
	}
	return (true);
}
