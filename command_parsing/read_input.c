/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 21:07:22 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/13 11:23:20 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		add_to_line(char **line, char buffer[BUFFER_SIZE + 1])
{
	char *aux;

	aux = *line;
	buffer[BUFFER_SIZE] = 0;
	*line = ft_strncat_in(*line, (char *)&buffer, BUFFER_SIZE);
	free(aux);
	return (0);
}

void	read_input(char **line)
{
	char	buffer[BUFFER_SIZE + 1];
	char	rd_buffer[2];
	int		i;
	char	*aux;

	i = 0;
	*line = NULL;
	rd_buffer[1] = 0;
	while (read(STDIN_FILENO, rd_buffer, 1))
	{
		if (rd_buffer[0] == '\n')
			break ;
		if (i == BUFFER_SIZE)
			i = add_to_line(line, buffer);
		buffer[i] = rd_buffer[0];
		i++;
	}
	buffer[i] = 0;
	aux = *line;
	*line = ft_strncat_in(*line, buffer, i);
	if (aux)
		free(aux);
}
