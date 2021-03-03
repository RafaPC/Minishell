/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_parsing_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 19:19:23 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/26 19:29:43 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_parsing_error(int return_value)
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
		ft_printf("syntax error near token '%s'\n", aux);
	}
	return (return_value);
}
