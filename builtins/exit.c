/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 15:20:50 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/19 10:48:43 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

/*
** Works like the normal atoi but uses a unsigned 64bits integer for the number
** and sets a boolean to true if the value goes beyond the maximum value for
** a signed 64bits integer
*/

int		exit_atoi(const char *str, t_bool *overflow, int i, u_int64_t number)
{
	int	sign;

	sign = 1;
	if (!str)
		return (0);
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
		sign *= (str[i++] == '-') ? -1 : 1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (number == 0)
			number = str[i] - 48;
		else
		{
			number = number * 10 + str[i] - 48;
			if (number > __INT64_MAX__)
			{
				*overflow = true;
				return (0);
			}
		}
		i++;
	}
	return ((int)number * sign);
}

/*
** Free all the memory and exits with a given status number
*/

void	free_and_exit(t_shell *shell, int exit_code)
{
	ft_lstdbl_clear(&shell->command_history, free);
	free_commands(shell->commands);
	ft_lstclear(&shell->env_list, free);
	exit(exit_code);
}

/*
** Sets the exit code and prints a messagge depending on some conditions
*/

void	check_exit(char **tokens, int *exit_code, int token_index,
t_bool overflow)
{
	if (tokens[1][token_index])
	{
		*exit_code = 2;
		ft_printf(STDERR_FILENO,
		"minishell: exit: %s: numeric argument required\n", tokens[1]);
	}
	else if (tokens[2])
	{
		*exit_code = 1;
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
	}
	else
	{
		*exit_code = exit_atoi(tokens[1], &overflow, 0, 0);
		if (overflow)
		{
			*exit_code = 255;
			ft_printf(STDERR_FILENO,
			"minishell: exit: %s: numeric argument required\n", tokens[1]);
		}
	}
}

/*
** Called when "exit" is written in the shell
** Frees the t_list* list of environment variables elements
** and the t_command *commands struct before exiting
*/

void	exit_command(t_shell *shell, int exit_code, int i)
{
	t_bool	overflow;
	int		signs;

	signs = 0;
	overflow = false;
	if (!shell->commands || !shell->commands->tokens[1])
		free_and_exit(shell, exit_code);
	while (shell->commands->tokens[1][i] &&
	(ft_strchr(" \t\f-+", shell->commands->tokens[1][i])||
	ft_isdigit(shell->commands->tokens[1][i])))
	{
		if (ft_isdigit(shell->commands->tokens[1][i]))
			exit_code = exit_atoi(&shell->commands->tokens[1][i], &overflow, 0, 0);
		if (shell->commands->tokens[1][i] == '+' || shell->commands->tokens[1][i] == '-')
			signs++;
		if (signs > 1)
			break ;
		i++;
	}
	check_exit(shell->commands->tokens, &exit_code, i, overflow);
	free_and_exit(shell, exit_code);
}
