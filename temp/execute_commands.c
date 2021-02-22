/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 12:22:16 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/22 20:55:51 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "ft_printf.h"

/* 
** Might need to change the permits!
*/

int get_input_and_output(char *file, int mode)
{
	int fd;

	if (mode == input_redirection)
	{
		if ((fd = open(file, O_RDONLY)) == -1)
			return(false);
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

t_bool is_builtin(char **command)
{
	if (!command[0])
		return (true);
	// if (!ft_strncmp(command, "echo", 5))
	// //	return (echo());
	// else if (!ft_strncmp(command, "cd", 3))
	// //	return (cd())
	// else if (!ft_strncmp(command, "pwd", 4))
	// //	return (pwd())
	// else if (!ft_strncmp(command, "export", 7))
	// 	//	return (export())
	// else if (!ft_strncmp(command, "unset", 6))
	// 	//	return (unset())
	// else if (!ft_strncmp(command, "env", 4))
	// 	//	return (env())
	return(false);
}

int command_execution(char **command, char **envp, int relation)
{
	int pid;
	int	wstatus;

	pid = 0;
	if (is_builtin(command)) //ERROR handling
		pid = 1;
	else if ((pid = fork()) == -1)
		ft_printf("Error al forkear");
	else if (pid == 0) // Hijo
	{
		//write(STDIN_FILENO, "Ejecuta\n", 8);
		execvp(command[0], command);
	}
	else
	{
		if (wait(&wstatus) == -1) // Error al esperar porque no haya hijos y cosas así
			ft_printf("Error al esperar");
		else if (WIFEXITED(wstatus))
		{
			// ft_putstr_fd("Returned normally\n", STDIN_FILENO);
			//ft_printf("Returned normally\n");
			//ft_printf("Code: %i\n", WEXITSTATUS(wstatus));
		}
		else if (WIFSIGNALED(wstatus))
		{
			// ft_putstr_fd("Returned by signal\n", STDIN_FILENO);
			ft_printf("Returned by signal\n");
			ft_printf("Signal code: %i\n", WTERMSIG(wstatus));
		}
		else
			ft_printf("Not returned normally\n");
	}
}

void	execute_commands(t_command *commands, char **envp)
{
	int			stdin_copy;
	int			stdout_copy;
	int			fdpipe[2];
	int			pid;
	
	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	while (commands->relation != simple_command)
	{
		if (!get_input_and_output(commands->tokens[0], commands->relation))
		{
			//Error al abrir algun fichero
		}
		else if (commands->relation == pipe_redirection)
		{
			pipe(fdpipe);
			dup2(fdpipe[1], STDOUT_FILENO);
			command_execution(commands->tokens, envp, commands->relation);
			dup2(fdpipe[0], STDIN_FILENO);
			close(fdpipe[0]);
			close(fdpipe[1]);
		}
		commands = commands->next;
	}
	command_execution(commands->tokens, envp, commands->relation);
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
}