/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 16:11:27 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/17 17:46:17 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**		Changes the current working directory
*/

t_bool	cd(t_list **env_list, char **args)
{
	char	*pwd_new;
	char	*aux;

	if (args[0] && args[1])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDOUT_FILENO);
		return (true);
	}
	aux = getcwd(NULL, 0);
	if (!args[0])//Hacer cd a home
		if ((pwd_new = get_env_var("HOME", *env_list)))//FIXME:checkear errores del chdir
		{
			chdir(pwd_new);
			free(pwd_new);
		}
		else
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			free(aux);
			errno = 1;
			return (true);
		}
	else if (chdir(args[0]) == -1)
	{
		if (errno == ENOENT)
			ft_printf(STDERR_FILENO,
			"minishell: cd: %s: No such file or directory\n", args[0]);
		else if (errno == EACCES)
		{
			ft_printf(STDERR_FILENO,
			"minishell: cd: %s: Permission denied\n", args[0]);
			errno = 1;
		}
		free(aux);
		return (false);
	}

	pwd_new = ft_strjoin("OLDPWD=", aux);
	export_variable(env_list, pwd_new);
	free(pwd_new);
	free(aux);

	aux = getcwd(NULL, 0);
	pwd_new = ft_strjoin("PWD=", aux);
	export_variable(env_list, pwd_new);
	free(aux);
	free(pwd_new);
	return (true);
}
