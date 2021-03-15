/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 12:57:24 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/15 01:20:49 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include <sys/types.h>

/*
** If command is a builtin it's executed and returns true, if not returns false
** Also, if the executes a builtin that modifies the environment list,
** the env_array is freed and created again so that it is updated
*/

int			is_builtin(
t_command *command, t_list **env_list, int *prev_exit_status)//FIXME: checkear lo que puede devolver cada builtin y eso
{
	int	result;

	result = -1;
	if (!ft_strncmp(command->tokens[0], "echo", 5))
		result = (echo(&command->tokens[1]));
	else if (!ft_strncmp(command->tokens[0], "cd", 3))
		result = (cd(env_list, &command->tokens[1]));
	else if (!ft_strncmp(command->tokens[0], "pwd", 4))
		result = (pwd());
	else if (!ft_strncmp(command->tokens[0], "export", 7))
		result = export(env_list, &command->tokens[1]);
	else if (!ft_strncmp(command->tokens[0], "unset", 6))
		result = unset(env_list, &command->tokens[1]);
	else if (!ft_strncmp(command->tokens[0], "env", 4))
		result = (env(*env_list, &command->tokens[1]));
	else if (!ft_strncmp(command->tokens[0], "exit", 5))
		ft_exit(command, env_list);
	*prev_exit_status = errno;
	return (result);
}

/*
** Checks wether the token is a directory or not using the opendir() function
*/

t_bool		is_directory(char *token, int *prev_exit_status)
{
	DIR *fd_dir;

	if (!ft_strncmp("..", token, 3))
	{
		ft_putstr_fd(
			"minishell: ..: command not found\n", STDERR_FILENO);
		*prev_exit_status = 127;
	}
	else if ((fd_dir = opendir(token)))
	{
		closedir(fd_dir);
		ft_printf(STDERR_FILENO,
			"minishell: %s: Is a directory\n", token);
		*prev_exit_status = 126;
		return (true);
	}
	errno = 0;
	return (false);
}

/*
** This function is calls if the first token starts with / or .
** It checks if the token can be opened as a file //TODO:
*/

t_bool		is_valid_path(char *path, int *prev_exit_status)
{
	int	fd;

	if ((fd = open(path, O_RDONLY)) != -1)
	{
		close(fd);
		return (true);
	}
	else if (errno == 13)
	{
		ft_printf(STDERR_FILENO,
		"minishell: %s: Permission denied\n", path);
		*prev_exit_status = 126;
		return (false);
	}
	else
	{
		ft_printf(STDERR_FILENO,
		"minishell: %s: No such file or directory\n", path);
		*prev_exit_status = 127;
		return (false);
	}
}
