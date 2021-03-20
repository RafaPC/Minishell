/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_io.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 16:15:36 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/20 17:08:30 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** This function is called when the input has an output redirection
** Depeding on the type of output redirections it opens the file
** one way or another, truncating its content or appeending to it
*/

t_bool	set_output(t_shell *shell, char *file, int mode)
{
	int		fd;

	if (mode == output_redirection)
		fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0666);
	else
		fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0666);
	if (fd == -1)
	{
		shell->prev_exit_status = errno;
		return (false);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		shell->prev_exit_status = errno;
		return (false);
	}
	close(fd);
	return (true);
}

/*
** TODO: Might need to change the permits!
*/

t_bool	set_input_and_output(t_shell *shell, int mode)
{
	int		fd;
	char	*file;

	file = shell->commands->tokens[0];
	parse_insertions(shell, true);
	if (mode == input_redirection)
	{
		if ((fd = open(file, O_RDONLY)) == -1)
		{
			shell->prev_exit_status = errno;
			return (false);
		}
		else if (dup2(fd, STDIN_FILENO) == -1)
		{
			shell->prev_exit_status = errno;
			return (false);
		}
		close(fd);
	}
	else if (mode == output_redirection || mode == output_redirection_app)
		if (!set_output(shell, file, mode))
			return (false);
	shell->prev_exit_status = errno;
	return (true);
}

/*
** This function handles a piped command by redirecting the standard output to
** a pipe write end before executing the command and then redirecting the
** standard input to the read end of a pipe so that the next command will read
** the first's command input when reading from standard input
*/

t_bool	handle_pipe_and_execute(t_shell *shell)
{
	int std_out_cpy;
	int	fdpipe[2];

	parse_insertions(shell, false);
	std_out_cpy = dup(STDOUT_FILENO);
	if (pipe(fdpipe) == -1)
		return (false);
	if (dup2(fdpipe[1], STDOUT_FILENO) == -1)
		return (false);
	command_execution(shell);
	if (errno)
		return (false);
	if (dup2(fdpipe[0], STDIN_FILENO) == -1)
		return (false);
	close(fdpipe[0]);
	close(fdpipe[1]);
	dup2(std_out_cpy, STDOUT_FILENO);
	return (true);
}
