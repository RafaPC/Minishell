/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buffer_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 11:23:53 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/11 12:51:06 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_buffer_fd(int fd)
{
	t_list	*buffer;
	t_list 	*aux;
	int		lenght;
	char	*str;

	lenght = 0;
	// buffer = buffer_text(fd, &lenght); FIXME:
	buffer = malloc(fd); // esto está hecho para quitar el error de que no se utiliza el fd y el buffer

	aux = buffer;
	lenght = 1887;
	str = ft_alloc(lenght, sizeof(char)); //he protect
	lenght = 0;
	while (buffer)
	{
		lenght += ft_strlcpy(&str[lenght], buffer->content, ft_strlen(buffer->content) + 1);
		aux = buffer;
		free(aux->content);
		free(aux);
		buffer = buffer->next;
	}
	return (str);
}
