/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 13:05:46 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/14 12:05:31 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** COMMAND: command linked structure
**
** FILE_NAME: name of the file to redict to.
**
** APPEND: whether the content is appended to the file or created anew.
**
** Auxiliary function wherein the acutal execution of the command takes place.
** Do note that it requires a function that iterates through the whole command execution pipeline.
** I used that verb becuse it needs to handle pipes... It is a joke :P
** 
** The execution of this function takes place on the child process and therefore, needs to write into the pipe,
** so the parent process may know the error code, if applicable.
**
** TODO: Check whether the file is created from scratch on append = false
*/

void	redirect_command(t_list *command, char *file_name, t_bool append,
int *fd)
{
	int file_fd;

	errno = 0;
	close(fd[0]);
	if (append)
		file_fd = open(file_name, O_CREAT | O_WRONLY | O_APPEND, 0666);
	else
		file_fd = open(file_name, O_CREAT | O_WRONLY, 0666);
	if (!errno)
	{
		dup2(file_fd, STDOUT_FILENO);
		printf("Hey listen"); // Command executing function, must return the errno of the thing or a 0 on no error;
	}
	close(file_fd);
	ft_putnbr_fd(errno, fd[1]);
	close(fd[1]);
}

/*
** COMMAND: command linked structure
**
** FILE_NAME: name of the file to redict to.
**
** APPEND: whether the content is appended to the file or created anew.
**
** Executes a command, redirecting the output to the passed file name.
** Returns the value of errno if a error is found or 0 otherwise.
**
** Note that the execution of the command takes place on a child process, and therefore,
** the funcion reads the error value of said execution from the pipe.
**command
** TODO figure out why its writing the number on the file :V
*/

int		redirect_output(t_list *command, char *file_name, t_bool append)
{
	int		aux;
	int		fd[2];
	char	*line;

	pipe(fd);
	if ((aux = fork()) == -1)
		return (-1);
	else if (!aux)
		redirect_command(command, file_name, append, fd);
	else
	{
		wait(NULL);
		close(fd[1]);
		get_next_line(fd[0], &line);
		aux = ft_atoi(line);
		close(fd[0]);
		free(line);
		return (aux);
	}
	return (0);
}

/*
**  COMMAND: command linked structure
**
** Launch when the relation of the command is 2;
** Iterates through the command list executing the command and redirecting the output to the files stored in the redirections command list elemnts.
** Note that the command is executed once per redirection element.
** Should an error be found during the execution, the while loop is stopped and the error value is returned. The super function should print the error and stop the exectution.
** Regardless of the above, the commands pointer is modified so it points to the next command to execute.
** 
** TODO: check that the command moving part is ok; It assumes that the command is over only when a relation of 0 is found.
** TODO: In theory, if an error is found, the command is still launched, leaving an empty file (which I think is what should happen?);
*/

int	handle_redirections(t_command **commands)
{
	t_command *cmd_to_exec;
	t_command *redirections;
	int aux;

	aux = 0;
	redirections = *commands;
	cmd_to_exec = *commands;
	while (cmd_to_exec->relation == 2 || cmd_to_exec->relation == 3)
		cmd_to_exec = redirections->next;
	while (redirections != cmd_to_exec)
	{
		aux = redirect_output(cmd_to_exec, redirections->command, redirections->relation - 2);
		redirections = redirections->next;
	}
	while (cmd_to_exec->relation != 0)
		cmd_to_exec = cmd_to_exec->next;
	*commands = cmd_to_exec->next;
	return (0);
}
