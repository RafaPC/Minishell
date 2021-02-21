/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 12:22:16 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/21 17:27:26 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

void execute_commands(t_command *commands)
{
	int 	tmpin;
	int 	tmpout;
	int 	fdin;
	int 	fdout;
	int		fdpipe[2];
	int		pid;
	
	tmpin = dup(STDIN_FILENO);
	tmpout = dup(STDOUT_FILENO);
	while (commands && commands->relation != 0)
	{
		if (commands->relation == input_redirection)
			fdin = open(*commands->command, O_RDONLY);
		else if (commands->relation == output_redirection)
			fdout = open(*commands->command, O_WRONLY | O_CREAT); //FIXME: O_TRUNC o algo así
		else if (commands->relation == pipe_redirection)
		{
			pipe(fdpipe);
			fdout = fdpipe[1];
			fdin = fdpipe[0];
		}
		if (commands->relation == simple_command|| commands->relation == pipe_redirection)
		{
			//Pongo los fd's correspondientes al stdin y stdout
			dup2(fdin, STDIN_FILENO);
			close(fdin);
			dup2(fdout, STDOUT_FILENO);
			close(fdout);
			if ((pid = fork()) == -1)
				printf("Error al forkear");
			if (pid == 0) // Hijo
				execvp(commands->command[0], commands->command);
			else //Padre
			{
				wait(NULL);
				dup2(tmpin, STDIN_FILENO);
				dup2(tmpout, STDOUT_FILENO);
			}
		}
		commands = commands->next;
	}
	close(tmpin);
	close(tmpout);
}