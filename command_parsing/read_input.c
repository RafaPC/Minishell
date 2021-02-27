/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 21:07:22 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/27 10:52:31 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_input(char **line)
{
	char	buffer[BUFFER_SIZE + 1];
	char	rd_buffer[2];
	int		i;
	char	*aux;

	i = 0;
	*line = NULL;
	rd_buffer[1] = 0;
	buffer[BUFFER_SIZE] = 0;
	while (read(STDIN_FILENO, rd_buffer, 1))
	{
		if (rd_buffer[0] == '\n')
			break ;
		if (i == BUFFER_SIZE)
		{
			aux = *line;
			buffer[BUFFER_SIZE] = 0;
			*line = ft_strncat_in(*line, (char *)&buffer, BUFFER_SIZE);
			free (aux);
			i = 0;
		}
		buffer[i] = rd_buffer[0];
		i++;
	}
	buffer[i] = 0;
	*line = ft_strncat_in(*line, buffer, i);
}