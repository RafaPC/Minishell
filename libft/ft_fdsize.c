/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fdsize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 11:13:05 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/08 18:46:00 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** FD = file descriptor of the file to get the lenght of.
** 
** Given a file descriptor, it will read its contents until the EOF, returning the
** lenght of said file.
** 
** Note:
** 	On an invalid file descriptor it will return -1.
**
** 	May be used to set the file pointer thingy to the end of file, thus allowing
** 	writing from the last content of the file. ">>" anyone?
**
** May also be used to buffer the entire file to memory, by getting the size and
** immediately doing a fd reset + read of lenght size. Something something cp. 
*/

int	ft_fdsize(int fd)
{
	char aux[BUFFER_SIZE + 1];
	int fd_aux;
	int i;
	int lenght;

    if (fd < 0 || !(fd_aux = dup(fd)))
		return (-1);
	lenght = 0;
	while ((i = read(fd, aux, BUFFER_SIZE)))
	{
		if (i == -1)
		{
			close (fd);
			return (-1);
		}
		aux[BUFFER_SIZE] = 0;
		lenght += i;
	}
	close(fd_aux);
	return (lenght);	
}