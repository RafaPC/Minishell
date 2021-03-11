/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_io.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 16:15:36 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/11 16:31:23 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** TODO: Might need to change the permits!
*/

int			get_input_and_output(
	char *file, int mode, int *prev_exit_status, t_list *env_list)
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
** TODO:
*/

t_command	*set_fd(t_command *commands, t_list **env_list, int *prev_exit_status)
{
	int			fdpipe[2];
	int			std_out_cpy;

	while (commands && commands->relation != simple_command)
	{
		if (!get_input_and_output(commands->tokens[0], commands->relation,
		prev_exit_status, *env_list))
			break ;
		else if (commands->relation == pipe_redirection)//TODO:Mover a una función aparte
		{
			parse_insertions(
				commands->tokens, *env_list, *prev_exit_status, false);
			std_out_cpy = dup(STDOUT_FILENO);
			if (pipe(fdpipe) == -1)
				break ;
			if (dup2(fdpipe[1], STDOUT_FILENO) == -1)
				break ;
			command_execution(commands, env_list, prev_exit_status);
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
