/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 16:11:27 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/21 13:00:26 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Sets PWD and OLDPWD enevironment variables
*/

void	cd_set_pwds(t_list **env_list, char *old_pwd)
{
	char *aux;
	char *new_pwd;

	aux = old_pwd;
	old_pwd = ft_strjoin("OLDPWD=", old_pwd);
	export_variable(env_list, old_pwd);
	free(old_pwd);
	free(aux);
	if ((aux = getcwd(NULL, 0)))
	{
		new_pwd = ft_strjoin("PWD=", aux);
		export_variable(env_list, new_pwd);
		free(aux);
		free(new_pwd);
	}
}

/*
** Tries to execute chdir() to a directory
** In case of error it prints the appropiate messages
*/

t_bool	cd_dir(char *directory)
{
	if (directory[0] == '\0')
		return (true);
	else if (chdir(directory) == -1)
	{
		if (errno == ENOENT)
			ft_printf(STDERR_FILENO,
			"minishell: cd: %s: No such file or directory\n", directory);
		else if (errno == EACCES)
		{
			ft_printf(STDERR_FILENO,
			"minishell: cd: %s: Permission denied\n", directory);
			errno = 1;
		}
		return (false);
	}
	return (true);
}

/*
** Tries to execute chdir() to the home directory
** If HOME doesn't exists it prints a message and sets errno to EPERM
*/

t_bool	cd_home(t_list *env_list)
{
	char	*home;
	t_bool	result;

	if ((home = get_env_var("HOME", env_list)))
	{
		result = cd_dir(home);
		free(home);
		return (result);
	}
	else
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		errno = EPERM;
		return (false);
	}
}

/*
** Tries to change the current directory to a new one and
** change PWD and OLDPWD accordingly
*/

void	cd(t_list **env_list, char **args)
{
	char	*old_pwd;

	if (args[0] && args[1])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDOUT_FILENO);
		return ;
	}
	else if (!(old_pwd = getcwd(NULL, 0)))
		return ;
	else if (!args[0])
	{
		if (!cd_home(*env_list))
		{
			free(old_pwd);
			return ;
		}
	}
	else if (!cd_dir(args[0]))
	{
		free(old_pwd);
		return ;
	}
	cd_set_pwds(env_list, old_pwd);
}
