/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 12:22:16 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/22 19:20:09 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "ft_printf.h"

/* 
** Might need to change the permits!
*/

int get_input_and_output(int *fd_in, int *fd_out, char *file, int mode)
{
	int fd;
	int aux;

	//TODO: obviamente cada open() puede dar error, hay que comprobar
	if (mode == input_redirection)
	{
		*fd_in = open(file, O_RDONLY);
		dup2(*fd_in, STDIN_FILENO);
	}
	else if (mode == output_redirection || mode == output_redirection_app)
	{
		if (mode == output_redirection)
			*fd_out = open(file, O_RDWR | O_CREAT | O_TRUNC, 664);
		else if (mode == output_redirection_app) //FIXME: esto se puede dejar como solo else, pero no se que queda mas claro
			*fd_out = open(file, O_RDWR | O_CREAT | O_APPEND, 664);
		dup2(*fd_out, STDOUT_FILENO);
	}
	return (true);
}

/*
** Executes the builtin command if found and return either 1 or errnum;
*/

t_bool is_builtin(char *command)
{
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

int command_execution(char **command, char **envp, int relation, t_fd_struct *fd)
{
	int pid;
	int	wstatus;

	pid = 0;
	if (is_builtin(command[0])) //ERROR handling
		pid = 1;
	else if ((pid = fork()) == -1)
		ft_printf("Error al forkear");
	else if (pid == 0) // Hijo
	{
		write(STDIN_FILENO, "Ejecuta\n", 8);
		execve(command[0], command, envp);
	}
	else
	{
		if (wait(&wstatus) == -1) // Error al esperar porque no haya hijos y cosas así
			ft_printf("Error al esperar");
		else if (WIFEXITED(wstatus))
		{
			// ft_putstr_fd("Returned normally\n", STDIN_FILENO);
			ft_printf("Returned normally\n");
			ft_printf("Code: %i\n", WEXITSTATUS(wstatus));
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

void	initialice_fd_struct(t_fd_struct *fds)
{
	fds->pipe_in = -1;
	fds->pipe_out = -1;
	fds->std_in_tmp = -1;
	fds->std_out_tmp = -1;
}

void	execute_commands(t_command *commands, char **envp)
{
	t_fd_struct	fd;
	int			fdpipe[2];
	int			pid;
	
	initialice_fd_struct(&fd);
	fd.stdin_copy = dup(STDIN_FILENO);
	fd.stdout_copy = dup(STDOUT_FILENO);
	while (commands->relation != simple_command)
	{
		if (get_input_and_output(&fd.std_in_tmp, &fd.std_out_tmp,
			commands->tokens[0], commands->relation))
		{
			//Error al abrir algun fichero
		}
		else if (commands->relation == pipe_redirection)
		{
			pipe(fdpipe);
			fd.pipe_in = fdpipe[0];
			fd.pipe_out = fdpipe[1];
			dup2(fd.pipe_out, STDOUT_FILENO);
			command_execution(commands->tokens, envp, commands->relation, &fd);
			dup2(fd.pipe_in, STDIN_FILENO);
		}
		commands = commands->next;
	}
	write(1, "Pasa por aqui\n", 14);
	command_execution(commands->tokens, envp, commands->relation, &fd);
	// Cerrar fd's y dejar como antes los standart input y output
	close(fd.std_in_tmp);
	close(fd.std_out_tmp);
	dup2(fd.stdin_copy, STDIN_FILENO);
	dup2(fd.stdout_copy, STDOUT_FILENO);
	close(fd.stdin_copy);
	close(fd.stdout_copy);
}