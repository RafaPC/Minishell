/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 18:49:51 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/09 22:35:37 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

t_list *buffer_text(int fd, int *lenght)
{
	char aux[BUFFER_SIZE];
	t_list *buffer;
	int i;
	int len;

	i = 0;
	buffer = 0;
	while ((i = read(fd, aux, BUFFER_SIZE)))
	{
		aux[i] = 0;
		ft_lstadd_back(&buffer, ft_lstnew(ft_strdup(aux)));
		len = len + i; //Change
	}
	*lenght = len;
	return (buffer);
}

int main (void)
{
	int fd = open("minishell.h", O_RDONLY);

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
	printf("%s\n", str);
}