/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 00:56:18 by rprieto-          #+#    #+#             */
/*   Updated: 2021/01/21 01:11:35 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tabs_to_spaces(char *string)
{
	while (*string)
	{
		if (*string == '\t')
			*string = ' ';
		else if (*string == '\n' && *(string + 1) == '\0')
			*string = '\0';
		string++;
	}
}
 
void	read_input(char *buffer)
{
	write(1, "Minishell-> ", 12);
	read(0, buffer, 1024);
	buffer[1024] = '\0';
}

void	empty_buffer(char *buffer)
{
	int i;

	i = 0;
	while (buffer[i])
	{
		buffer[i] = '\0';
		i++;
	}
}
