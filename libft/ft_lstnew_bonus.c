/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 18:40:23 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/20 00:23:33 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstnew(void *content)
{
	t_list *new_elem;

	if (!content)
		return (NULL);
	if ((new_elem = (t_list*)malloc(sizeof(t_list))) == NULL)
	{
		free(content);
		return (NULL);
	}
	new_elem->content = (void*)content;
	new_elem->next = NULL;
	return (new_elem);
}
