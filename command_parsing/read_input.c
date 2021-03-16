/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 21:07:22 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/15 16:20:49 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	read_input(char **line)
{
	char	buffer[BUFFER_SIZE + 1];
	char	rd_buffer[1];
	int		i;
	char	*aux;
	int		bytes_read;

	i = 0;
	*line = NULL;
	while (true)
	{
		bytes_read = read(STDIN_FILENO, rd_buffer, 1);
		if (i == BUFFER_SIZE || !bytes_read || rd_buffer[0] == '\n')
		{
			aux = *line;
			buffer[i] = 0;
			*line = ft_strncat_in(*line, (char *)&buffer, i + 1);
			if (aux)
				free(aux);
			i = 0;
		}
		if (!bytes_read || rd_buffer[0] == '\n')
			return (bytes_read ? true : false);
		buffer[i++] = rd_buffer[0];
	}
	return (true);
}
