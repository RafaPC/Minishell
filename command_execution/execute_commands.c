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

t_command	*execute_commands(t_command *commands,
t_list **env_list, int *prev_exit_status)
{
	int	stdin_copy;
	int	stdout_copy;

	errno = 0;
	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	while (commands && commands->relation != simple_command)
	{
		if (!get_input_and_output(commands->tokens[0], commands->relation,
		prev_exit_status, *env_list))
			break ;
		else if (commands->relation == pipe_redirection)
			if (!handle_pipe_and_execute(commands, env_list, prev_exit_status))
				break ;
		commands = del_command(commands);
	}
	if (errno)
		commands = print_redirection_errors(commands, prev_exit_status);
	else
	{
		parse_insertions(commands->tokens, *env_list, *prev_exit_status, false);
		command_execution(commands, env_list, prev_exit_status);
	}
	restore_fds(stdin_copy, stdout_copy);
	return (del_command(commands));//I think this needs to be updated to while != simple command, free_command.
}

/*
** TODO:
*/

void		command_execution(t_command *command, t_list **env_list,
int *prev_exit_status)
{
	int			pid;
	int			child_tatus;
	t_bool		is_valid_file;

	is_valid_file = false;
	pid = 0;
	if (!command->tokens[0] ||
	is_builtin(command, env_list, prev_exit_status) != -1 ||
	is_directory(command->tokens[0], prev_exit_status))
		return ;
	if (ft_checkchar(command->tokens[0][0], "/.") &&
	!(is_valid_file = is_valid_path(command->tokens[0], prev_exit_status)))
		return ;
	if ((pid = fork()) == 0)
		child_process(command, is_valid_file, env_list, prev_exit_status);
	else if (pid > 0 && wait(&child_tatus) != -1)
	{
		if (WIFEXITED(child_tatus))
			*prev_exit_status = WEXITSTATUS(child_tatus);
		else if (WIFSIGNALED(child_tatus))
			*prev_exit_status = WTERMSIG(child_tatus);
	}
}

/*
** This function is executed in a child process
** It executes a command with execve() and frees memory
** If the first token is a file path, it is executed directly
** If not, it is appended to every path of the environment variable PATH
** to check if it's the name of an executable
*/

void		child_process(t_command *command, t_bool is_file_path,
t_list **env_list, int *prev_exit_status)
{
	char	*command_path;
	char	**env_array;

	env_array = env_list_to_array(*env_list);
	if (is_file_path)
		execve(command->tokens[0], command->tokens, env_array);
	else if ((command_path = get_command_path(
		get_path(*env_list), command->tokens[0])) != NULL)
	{
		execve(command_path, command->tokens, env_array);
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
	free(env_array);
	free_commands(command);
	exit(*prev_exit_status);
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
