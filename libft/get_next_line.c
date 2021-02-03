/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 10:18:19 by user42            #+#    #+#             */
/*   Updated: 2020/10/24 17:22:05 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_gnl	*find_struct(t_gnl *str, int fd)
{
	t_gnl *temp;

	temp = 0;
	if (str)
		while (str && str->fd != fd)
		{
			temp = str;
			str = str->next;
		}
	if (!str)
	{
		if (!(str = malloc(sizeof(t_gnl))))
			return (0);
		str->fd = fd;
		str->next = 0;
		str->line = 0;
		if (temp)
			temp->next = str;
	}
	return (str);
}

static int		free_struct(t_gnl **str, t_gnl *ptr)
{
	t_gnl *str_temp;

	str_temp = *str;
	if (str_temp == 0 || ptr == 0)
		return (-1);
	if (str_temp != ptr)
	{
		while (str_temp->next != ptr)
			str_temp = str_temp->next;
		str_temp->next = ptr->next;
	}
	else if (str_temp->next)
		*str = str_temp->next;
	if (ptr->line)
		free(ptr->line);
	free(ptr);
	if (ptr == *str)
		*str = 0;
	return (-1);
}

static int		create_line(char **line, t_gnl **str, int bytes_read, int fd)
{
	char	*p;
	t_gnl	*str_pt;

	str_pt = find_struct(*str, fd);
	if (bytes_read == -1)
		return (free_struct(str, str_pt));
	if ((p = ft_strchr(str_pt->line, 10)))
	{
		if (!(*line = ft_strncat_in(0, str_pt->line, p - (char*)str_pt->line)))
			return (-1);
		if (!(p = ft_strncat_in(p + 1, 0, 0)))
			return (-1);
		free(str_pt->line);
		str_pt->line = p;
	}
	else
	{
		if (!(*line = ft_strncat_in(str_pt->line, 0, 0)))
			return (0);
		free_struct(str, str_pt);
		return (0);
	}
	return (1);
}

static int		read_line(t_gnl **str, int fd)
{
	int			bytes_read;
	char		aux[BUFFER_SIZE + 1];
	t_gnl		*str_temp;
	char		*p;

	while ((bytes_read = read(fd, aux, BUFFER_SIZE)) > 0)
	{
		aux[bytes_read] = 0;
		if (*str == 0)
			*str = find_struct(*str, fd);
		str_temp = find_struct(*str, fd);
		if (!(p = ft_strncat_in(str_temp->line, aux, bytes_read)))
			return (-1);
		if (str_temp->line)
			free(str_temp->line);
		str_temp->line = p;
		if (ft_strchr(aux, 10))
			break ;
	}
	return (bytes_read);
}

int				get_next_line(int fd, char **line)
{
	int				bytes_read;
	static t_gnl	*str = 0;

	if (fd < 0 || !line)
		return (-1);
	bytes_read = read_line(&str, fd);
	if (bytes_read == 0 && !str)
	{
		*line = ft_strncat_in(0, 0, 0);
		return (0);
	}
	return (create_line(line, &str, bytes_read, fd));
}
