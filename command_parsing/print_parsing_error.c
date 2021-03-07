/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_parsing_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 19:19:23 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/07 11:58:53 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_parsing_error(int return_value, int *prev_exit_status)
{
	char *aux;

	aux = 0;
	if (return_value)
	{
		if (return_value == input_redirection)
			aux = "<";
		else if (return_value == pipe_redirection)
			aux = "|";
		else if (return_value == output_redirection)
			aux = ">";
		else if (return_value == semicolon)
			aux = ";";
		else if (return_value == newline)
			aux = "newline";
		ft_putstr_fd("syntax error near unexpected token \'", STDERR_FILENO);
		ft_putstr_fd(aux, STDERR_FILENO);
		ft_putstr_fd("\'\n", STDERR_FILENO);
		*prev_exit_status = 1;
	}
	return (return_value);
}
