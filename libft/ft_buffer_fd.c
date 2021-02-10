/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buffer_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 11:23:53 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/10 20:30:59 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char *ft_buffer_fd(int fd)
{
	t_list	*buffer;
	t_list 	*aux;
	int		lenght;
	char	*str;

	lenght = 0;
	buffer = buffer_text(fd, &lenght);
	aux = buffer;
	lenght = 1887;
	str = ft_alloc(lenght, sizeof(char)); //protect
	lenght = 0;
	while (buffer)
	{
		lenght += ft_strlcpy(&str[lenght], buffer->content, ft_strlen(buffer->content) + 1);
		aux = buffer;
		free(aux->content);
		free(aux);
		buffer = buffer->next;
	}