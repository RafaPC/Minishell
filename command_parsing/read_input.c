/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 21:07:22 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/26 21:29:00 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void read_input(char **line)
{
	char buffer[BUFFER_SIZE + 2];
	t_bool line_jump;
	line_jump = 0;
	int i;
	
	i = 2;
	buffer[BUFFER_SIZE + 1] = 0;
	while (line_jump)
	{
		if (!read(STDIN_FILENO, buffer, 1))
			break ;
		if (buffer[0] == '\n')
		{
			line_jump = true;
			break ;
		}
		if (i == BUFFER_SIZE + 2)
		{
			*line = ft_strncat_in(*line, &buffer[2], BUFFER_SIZE);
			ft_memset(&buffer[2], 0, BUFFER_SIZE);
		}
		buffer[i] = buffer[0];
		i++;
	}
	if(!line_jump)
		*line = ft_strncat_in(*line, &buffer[2], BUFFER_SIZE);
}