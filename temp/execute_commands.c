/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 12:22:16 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/28 11:22:54 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "ft_printf.h"

/*
** Might need to change the permits!
*/

int			get_input_and_output(char *file, int mode)
{
	int fd;

	if (mode == input_redirection)
	{
		if ((fd = open(file, O_RDONLY)) == -1)
			return (false);
		if (dup2(fd, STDIN_FILENO) == -1)
			return (false);
		close(fd);
	}
	else if (mode == output_redirection || mode == output_redirection_app)
	{
		if (mode == output_redirection)
			fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0666);
		else if (mode == output_redirection_app)
			fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0666);
		if (fd == -1)
			return (false);
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (false);
		close(fd);
	}
	return (true);
}

/*
** Executes the builtin command if found and return either 1 or errnum;
*/

int			is_builtin(t_command *command, char ***env_array, t_list **env_list)
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
		ft_exit(command, env_list, *env_array);//ERROR liberar memoria y un monton de cosas
	if (result == 2)
	{
		free(*env_array);
		*env_array = env_list_to_array(*env_list);
	}
	return (result);
}

/*
** TODO:	COSILLAS A TENER EN CUENTA
** checkear si el comando empieza por / eso significa que hay que ejecutarlo
** directamente sin checkear si es builtin o un comando que buscar en el path
** si tiene una / pero no en el primer carácter, concatenarlo al directorio
** actual y checkear si existe el archivo
*/

void		command_execution(t_command *command, char ***env_array,
t_list **env_list)
{
	int		pid;
	int		wstatus;
	char	*command_path;

	pid = 0;
	if (is_builtin(command, env_array, env_list) != -1)//ERROR handling
		pid = 1;
	else if ((pid = fork()) == -1)
		ft_printf("Error al forkear");
	else if (pid == 0)// Hijo
	{
		if ((command_path = get_command_path(
			get_path(*env_list), command->tokens[0])) != NULL)
		{
			execve(command_path, command->tokens, *env_array);
			free(command_path);
		}
		else
		{
			errno = 0;
			ft_printf("%s: command not found\n", command->tokens[0]);
		}
		ft_lstclear(env_list, free);
		free(*env_array);
		free_commands(command);
		exit(0);
	}
	else
	{
		if (wait(&wstatus) == -1)// Error al esperar porque no haya hijos y cosas así
			ft_printf("Error al esperar");
		else if (WIFEXITED(wstatus))
		{
			//ft_printf("Returned normally\n");
			//ft_printf("Code: %i\n", WEXITSTATUS(wstatus));
		}
		else if (WIFSIGNALED(wstatus))
		{
			ft_printf("Returned by signal\n");
			ft_printf("Signal code: %i\n", WTERMSIG(wstatus));
		}
		else
			ft_printf("Not returned normally\n");
	}
}

t_command	*set_fd(t_command *commands, char ***env_array, t_list **env_list)
{
	int			fdpipe[2];
	int			std_out_cpy;

	while (commands->relation != simple_command)
	{
		if (!get_input_and_output(commands->tokens[0], commands->relation))
			break ;
		else if (commands->relation == pipe_redirection)//Mover a una función aparte
		{
			std_out_cpy = dup(STDOUT_FILENO);
			if (pipe(fdpipe) == -1)
				break ;
			if (dup2(fdpipe[1], STDOUT_FILENO) == -1)
				break ;
			command_execution(commands, env_array, env_list);
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
	return (commands);
}

t_command	*execute_commands(t_command *commands, char ***env_array,
t_list **env_list)
{
	int			stdin_copy;
	int			stdout_copy;

	errno = 0;
	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	commands = set_fd(commands, env_array, env_list);
	if (!errno)
		command_execution(commands, env_array, env_list);
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
	return (errno ? handle_errors(commands) : del_command(commands));
}
