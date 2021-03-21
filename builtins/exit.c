/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 15:20:50 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/21 16:31:12 by rprieto-         ###   ########.fr       */
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
	if (shell->command_history)
		ft_lstdbl_clear(&(shell->command_history), free);
	if (shell->copy_line)
		free(shell->copy_line);
	free_commands(shell->commands);
	ft_lstclear(&shell->env_list, free);
	exit(exit_code);
}

/*
** Sets the exit code and prints a messagge depending on some conditions
*/

void	check_exit(char **tokens, int *exit_code, char last_character,
t_bool overflow)
{
	if (last_character)
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
** Checks if there are no tokens, the token is null or the token is a null char
** If on the conditions is true exits the program with the appropiate number
*/

void	check_token_errors(t_shell *shell)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!shell->commands || !shell->commands->tokens[1])
		free_and_exit(shell, 0);
	else if (shell->commands->tokens[1][0] == '\0')
	{
		ft_printf(STDERR_FILENO,
			"minishell: exit: : numeric argument required\n");
		free_and_exit(shell, 2);
	}
}

/*
** Called when "exit" is written in the shell
** Frees the t_list* list of environment variables elements
** and the t_command *commands struct before exiting
*/

void	exit_command(t_shell *shell)
{
	t_bool	overflow;
	int		signs;
	char	*c;
	int		exit_code;

	exit_code = 0;
	signs = 0;
	overflow = false;
	check_token_errors(shell);
	c = shell->commands->tokens[1];
	while (*c && (ft_strchr(" \t\f+-", *c) || ft_isdigit(*c)))
	{
		if (ft_isdigit(*c))
			exit_code = exit_atoi(c, &overflow, 0, 0);
		if (*c == '+' || *c == '-')
			signs++;
		if (signs > 1)
			break ;
		c++;
	}
	check_exit(shell->commands->tokens, &exit_code, *c, overflow);
	free_and_exit(shell, exit_code);
}
