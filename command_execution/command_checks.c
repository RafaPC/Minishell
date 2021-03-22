/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 12:57:24 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/22 09:58:42 by rprieto-         ###   ########.fr       */
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

t_bool		is_builtin(t_shell *shell, char *token)
{
	if (!ft_strncmp(token, "echo", 5))
		echo(&shell->commands->tokens[1], 1);
	else if (!ft_strncmp(token, "cd", 3))
		cd(&shell->env_list, &shell->commands->tokens[1]);
	else if (!ft_strncmp(token, "pwd", 4))
		pwd();
	else if (!ft_strncmp(token, "export", 7))
		export(&shell->env_list, &shell->commands->tokens[1], 0);
	else if (!ft_strncmp(token, "unset", 6))
		unset(&shell->env_list, &shell->commands->tokens[1]);
	else if (!ft_strncmp(token, "env", 4))
		env(shell->env_list, shell->commands->tokens[1]);
	else if (!ft_strncmp(token, "exit", 5))
		exit_command(shell);
	else
		return (false);
	shell->prev_exit_status = errno;
	return (true);
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
		return (true);
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
** It checks if the token can be opened as a file
** If it can't be opened, it prints a message depending on the errno value
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
