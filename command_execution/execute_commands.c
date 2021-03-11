/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 12:22:16 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/11 16:24:13 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
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
**
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
**
*/

void		command_execution(t_command *command, t_list **env_list,
int *prev_exit_status)
{
	int			pid;
	t_bool		is_valid_file;

	is_valid_file = false;
	pid = 0;
	if (!command->tokens[0] ||
	is_builtin(command, env_list, prev_exit_status) != -1 ||
	is_directory(command->tokens[0], prev_exit_status))//ERROR handling
		return ;
	if (ft_checkchar(command->tokens[0][0], "/.") &&
	!(is_valid_file = is_valid_path(command->tokens[0], prev_exit_status)))
			return ;
	if ((pid = fork()) == -1)
		ft_printf(STDOUT_FILENO, "Fork error\n");
	else if (pid == 0)
		child_process(command, is_valid_file, env_list, prev_exit_status);
	else
		wait_child_status(prev_exit_status);
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
	else
	{//FIXME:maybe cambiar lo de los errores para solo poner el prev_exit_status
		if (errno == 13)
		{
			ft_printf(STDERR_FILENO,
			"minishell: %s: Permission denied\n", path);
			*prev_exit_status = 126;
		}
		ft_printf(STDERR_FILENO,
		"minishell: %s: No such file or directory\n", path);
		*prev_exit_status = 127;
		return (false);
	}
}

/*
** This function is executed in a child process
** It executes a command with execve() and frees memory
** If the first token is a file path, it is executed directly
** If not, it is appended to every path of the environment variable PATH
** to check if it's the name of an executable
*/

void		child_process(t_command *command, t_bool is_file_path,
t_list **env_list, int *prev_exit_status)
{
	char	*command_path;
	char	**env_array;

	env_array = env_list_to_array(*env_list);
	if (is_file_path)
		execve(command->tokens[0], command->tokens, env_array);
	else if ((command_path = get_command_path(
		get_path(*env_list), command->tokens[0])) != NULL)
	{
		execve(command_path, command->tokens, env_array);
		free(command_path);
	}
	else
	{
		errno = 0;
		ft_printf(STDERR_FILENO,
		"minishell: %s: command not found\n", command->tokens[0]);
		*prev_exit_status = 127;
	}
	ft_lstclear(env_list, free);
	free(env_array);
	free_commands(command);
	exit(*prev_exit_status);
}

/*
** Waits for the child process and gets its exit status
*/

void		wait_child_status(int *prev_exit_status)
{
	int	wstatus;

	if (wait(&wstatus) == -1)//Error al esperar porque no haya hijos y cosas así
		ft_printf(STDOUT_FILENO, "Error al esperar");
	else if (WIFEXITED(wstatus))
		*prev_exit_status = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
		*prev_exit_status = WTERMSIG(wstatus);
	else
		ft_printf(STDOUT_FILENO, "Not returned normally\n");
}

/*
** TODO:
*/

t_command	*execute_commands(t_command *commands,
t_list **env_list, int *prev_exit_status)
{
	int	stdin_copy;
	int	stdout_copy;

	errno = 0;
	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	commands = set_fd(commands, env_list, prev_exit_status);
	if (!errno)
	{
		parse_insertions(commands->tokens, *env_list, *prev_exit_status, false);
		command_execution(commands, env_list, prev_exit_status);
	}
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
	return (del_command(commands));
	//I think this needs to be updated to while != simple command, free_command.
}
