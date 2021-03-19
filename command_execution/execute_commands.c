/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 12:22:16 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/15 22:54:32 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

/*
** TODO:
*/

t_command	*execute_commands(t_shell *shell)
{
	int	stdin_copy;
	int	stdout_copy;

	errno = 0;
	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	while (shell->commands && shell->commands->relation != simple_command)
	{
		if (!get_input_and_output(shell, shell->commands->relation))
			break ;
		else if (shell->commands->relation == pipe_redirection &&
		!handle_pipe_and_execute(shell))
				break ;
		shell->commands = del_command(shell->commands);
	}
	if (errno)
		shell->commands = print_redirection_errors(shell->commands, &shell->prev_exit_status);
	else
	{
		parse_insertions(shell, false);
		command_execution(shell);
	}
	restore_fds(stdin_copy, stdout_copy);
	return (del_command(shell->commands));
}

/*
** TODO:
*/

void		command_execution(t_shell *shell)
{
	int			pid;
	int			child_tatus;
	t_bool		is_valid_file;

	is_valid_file = false;
	pid = 0;
	if (!shell->commands->tokens[0] || is_builtin(shell) != -1 ||
	is_directory(shell->commands->tokens[0], &shell->prev_exit_status))
		return ;
	if (ft_checkchar(shell->commands->tokens[0][0], "/.") &&
	!(is_valid_file = is_valid_path(shell->commands->tokens[0], &shell->prev_exit_status)))
		return ;
	if ((pid = fork()) == 0)
		child_process(shell, is_valid_file);
	else if (pid > 0 && wait(&child_tatus) != -1)
	{
		if (WIFEXITED(child_tatus))
			shell->prev_exit_status = WEXITSTATUS(child_tatus);
		else if (WIFSIGNALED(child_tatus))
			shell->prev_exit_status = WTERMSIG(child_tatus);
	}
}

/*
** This function is executed in a child process
** It executes a command with execve() and frees memory
** If the first token is a file path, it is executed directly
** If not, it is appended to every path of the environment variable PATH
** to check if it's the name of an executable
*/

void		child_process(t_shell *shell, t_bool is_file_path)
{
	char	*command_path;
	char	**env_array;

	env_array = env_list_to_array(shell->env_list);
	if (is_file_path)
		execve(shell->commands->tokens[0], shell->commands->tokens, env_array);
	else if ((command_path = get_command_path(
		get_path(shell->env_list), shell->commands->tokens[0])) != NULL)
	{
		execve(command_path, shell->commands->tokens, env_array);
		free(command_path);
	}
	else
	{
		errno = 0;
		ft_printf(STDERR_FILENO,
		"minishell: %s: command not found\n", shell->commands->tokens[0]);
		shell->prev_exit_status = 127;
	}
	ft_lstclear(&shell->env_list, free);
	free(env_array);
	free_commands(shell->commands);
	ft_lstdbl_clear(&shell->command_history, free);
	exit(shell->prev_exit_status);
}

/*
** Restore stdin and stdout fds
*/

void		restore_fds(int stdin_copy, int stdout_copy)
{
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
}
