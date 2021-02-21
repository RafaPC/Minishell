/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 12:22:16 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/21 19:20:30 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

/* 
** Might need to change the permits!
*/

int get_input_and_output(int *fd_in, int *fd_out, char *file, int mode)
{
	int fd;
	int aux;

	if (mode = input_redirection)
	{
		fd = STDIN_FILENO;
		aux = open(file, O_RDONLY);
	}
	else
	{
		fd = STDOUT_FILENO;
		if (mode = output_redirection)
			aux = open (file, O_RDWR | O_CREAT | O_TRUNC);
		else if (mode = output_redirection_app)
			aux = open (file, O_RDWR | O_CREAT | O_APPEND);
		else
			return(-2);
	}
	if (dup2(fd, dup2(aux, fd)) == -1 || fd == -1)
		return (-1);
	if (mode != input_redirection)
		*fd_out = fd;
	else
		*fd_in = fd;
	return(true);
}

/*
** Executes the builtin command if found and return either 1 or errnum;
*/

t_bool is_builtin(char *command)
{
	if (!ft_strncmp(command, "echo", 5))
	//	return (echo);
	else if (!ft_strncmp(command, "cd", 3))
	//	return (cd)
	else if (!ft_strncmp(command, "pwd", 4))
	//	return (cd)
	else if (!ft_strncmp(command, "export", 7))
		//	return (cd)
	else if (!ft_strncmp(command, "unset", 6))
		//	return (cd)
	else if (!ft_strncmp(command, "env", 4))
		//	return (cd)
	return(0);
}

int command_execution(char **command)
{
	int pid;

	pid = 0;
	if (is_builtin(command[0])) //ERROR handling
		pid = 1;
	if (!pid && (pid = fork()) == -1)
		printf("Error al forkear");
	else if (pid == 0) // Hijo
		execvp(commands->command[0], commands->command);
	else
	{
		wait(NULL);
		if (commands->relation != pipe_redirection)
			dup2(fd[stdin_bkp], STDIN_FILENO);
		dup2(fd[stdout_bkp], STDOUT_FILENO);
	}
}

void execute_commands(t_command *commands)
{
	int		fd[6];
	int		fdpipe[2];
	int		pid;
	
	fd[stdin_bkp] = dup(STDIN_FILENO);
	fd[stdout_bkp] = dup(STDOUT_FILENO);
	while (commands && commands->relation != 0)
	{
		if (get_input_and_output(&fd[std_in_tmp], &fd[std_out_tmp],
			commands->command[0], commands->relation))
			commands = commands->next;
		else
		{
			if (commands->relation == pipe_redirection)
			{
				pipe(&fd[pipe_in]);
				fd[pipe_in] = dup2(fd[pipe_in], STDOUT_FILENO);
				fd[std_in_tmp] = fd[pipe_out];
			}
			commands = commands->next;
		}
	}
	close(fd[std_in_tmp]);
	close(fd[std_out_tmp]);
}