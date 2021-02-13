/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extract.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/07 13:28:47 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/13 12:03:22 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** LINE = string to which characters will be removed.
**
** INDEX = Marks where in *line the characters will be removed.
**
** LENGHT = how many characters will be removed.
**
** Inserts lenght characters of addendum in the string line, creating a new one.
** Returns an index pointing to the character after the inserted bit.
** Removes lenght characters of the string line, creating a new one.
** Returns an index pointing to the character after the removed bit.
**
** NOTE:
** *line is assumed to be malloced.
*/

int	ft_extract(char **line, unsigned int index, unsigned int lenght)
{
	char	*aux;
	int		i;

	if (!(aux = malloc(ft_strlen(*line) - lenght + 1)))
		return (0);
	i = lenght < index + 1 ? index + 1 - lenght : 0;
	ft_strlcpy(aux, *line, i + 1);
	ft_strlcpy(&aux[i], &(*line)[index + 1], ft_strlen(&(*line)[index]));
	//free(*line);
	*line = aux;
	return (i);
}
