/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 12:22:16 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/08 15:39:59 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <dirent.h>
#include <sys/types.h>

/*
** Might need to change the permits!
*/

int			get_input_and_output(char *file, int mode, int *prev_exit_status, t_list *env_list)
{
	int fd;

	parse_insertions(&file, env_list, *prev_exit_status, true);
	if (mode == input_redirection)
	{
		if ((fd = open(file, O_RDONLY)) == -1 && (*prev_exit_status = errno))
			return (false);
		if (dup2(fd, STDIN_FILENO) == -1 && (*prev_exit_status = errno))
			return (false);
		close(fd);
	}
	else if (mode == output_redirection || mode == output_redirection_app)
	{
		if (mode == output_redirection)
			fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0666);
		else if (mode == output_redirection_app)
			fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0666);
		if (fd == -1 && (*prev_exit_status = errno))
			return (false);
		if (dup2(fd, STDOUT_FILENO) == -1 && (*prev_exit_status = errno))
			return (false);
		close(fd);
	}
	*prev_exit_status = errno;
	return (true);
}

/*
** Executes the builtin command if found and return either 1 or errnum;
*/

int			is_builtin(t_command *command, char ***env_array, t_list **env_list, int *prev_exit_status)
{
	int	result;

	result = -1;
	if (!command->tokens[0])
		result = true;
	else if (!ft_strncmp(command->tokens[0], "echo", 5))
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
		ft_exit(command, env_list, *env_array);
	if (result == 2)
	{
		free(*env_array);
		*env_array = env_list_to_array(*env_list);
	}
	*prev_exit_status = errno;
	return (result);
}

t_bool		execute_path(t_command *command, int *prev_exit_status)
{
	DIR *fd_dir;

	if (!ft_strncmp("..", command->tokens[0], 3))
	{
		ft_putstr_fd(
			"minishell: ..: command not found\n",STDERR_FILENO);
		*prev_exit_status = 127;
	}
	else if((fd_dir = opendir(command->tokens[0])))
	{
		closedir(fd_dir);
		ft_printf(STDERR_FILENO,
			"minishell: %s: Is a directory\n", command->tokens[0]);
		*prev_exit_status = 126;
		return (true);
	}
	errno = 0;
	return (false);
}

/*
** TODO:	COSILLAS A TENER EN CUENTA
** checkear si el comando empieza por / eso significa que hay que ejecutarlo
** directamente sin checkear si es builtin o un comando que buscar en el path
** si tiene una / pero no en el primer carácter, concatenarlo al directorio
** actual y checkear si existe el archivo
*/

void		command_execution(t_command *command, char ***env_array,
t_list **env_list, int *prev_exit_status)
{
	int		pid;
	int		wstatus;
	char	*command_path;
	int		fd;

	fd = -1;
	pid = 0;

	if (is_builtin(command, env_array, env_list, prev_exit_status) != -1 || execute_path(command, prev_exit_status))//ERROR handling
		return ;
	if (ft_checkchar(command->tokens[0][0], "/."))
	{
		if ((fd = open(command->tokens[0], O_RDONLY)) != -1)
			close(fd);
		else
		{//FIXME: maybe cambiar lo de los erroes de forma que aquí solo haga falta poner el prev_exit_status
			ft_printf(STDERR_FILENO, "minishell: %s: No such file or directory\n", command->tokens[0]);
			*prev_exit_status = 127;
			return ;
		}
	}
	if ((pid = fork()) == -1)
		ft_printf(STDOUT_FILENO, "Error al forkear");
	else if (pid == 0)// Hijo
	{
		if (fd != -1)
			execve(command->tokens[0], command->tokens, *env_array);
		else if ((command_path = get_command_path(
			get_path(*env_list), command->tokens[0])) != NULL)
		{
			execve(command_path, command->tokens, *env_array);
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
		free(*env_array);
		free_commands(command);
		exit(*prev_exit_status);
	}
	else
	{
		if (wait(&wstatus) == -1)// Error al esperar porque no haya hijos y cosas así
			ft_printf(STDOUT_FILENO, "Error al esperar");
		else if (WIFEXITED(wstatus))
			*prev_exit_status = WEXITSTATUS(wstatus);
		else if (WIFSIGNALED(wstatus))
		{
			ft_printf(STDOUT_FILENO, "Returned by signal\n");
			ft_printf(STDOUT_FILENO, "Signal code: %i\n", WTERMSIG(wstatus));
		}
		else
			ft_printf(STDOUT_FILENO, "Not returned normally\n");
	}
}

t_command	*set_fd(t_command *commands, char ***env_array, t_list **env_list, int *prev_exit_status)
{
	int			fdpipe[2];
	int			std_out_cpy;

	while (commands && commands->relation != simple_command)
	{
		if (!get_input_and_output(commands->tokens[0], commands->relation, prev_exit_status, *env_list))
			break ;
		else if (commands->relation == pipe_redirection)//Mover a una función aparte
		{
			parse_insertions(commands->tokens, *env_list, *prev_exit_status, false);
			std_out_cpy = dup(STDOUT_FILENO);
			if (pipe(fdpipe) == -1)
				break ;
			if (dup2(fdpipe[1], STDOUT_FILENO) == -1)
				break ;
			command_execution(commands, env_array, env_list, prev_exit_status);
			if (errno)
				break ;
			if (dup2(fdpipe[0], STDIN_FILENO) == -1)
				break ;
			close(fdpipe[0]);
			close(fdpipe[1]);
			dup2(std_out_cpy, STDOUT_FILENO);
		}
		commands = del_command(commands);
	}
	if (errno)
		commands = print_redirection_errors(commands, prev_exit_status); 
	return (commands);
}

t_command	*execute_commands(t_command *commands, char ***env_array,
t_list **env_list, int *prev_exit_status)
{
	int			stdin_copy;
	int			stdout_copy;
	errno = 0;
	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	commands = set_fd(commands, env_array, env_list, prev_exit_status);
	if (!errno)
	{
		parse_insertions(commands->tokens, *env_list, *prev_exit_status, false);
		command_execution(commands, env_array, env_list, prev_exit_status);
	}
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
	return (del_command(commands)); //I think this needs to be updated to while != simple command, free_command.
}
