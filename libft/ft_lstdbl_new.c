/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdbl_new.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 00:35:41 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/20 00:55:47 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list_dbl	*ft_lstdbl_new(void *content)
{
	t_list_dbl *new_elem;

	if (!content)
		return (NULL);
	if ((new_elem = (t_list_dbl*)malloc(sizeof(t_list_dbl))) == NULL)
	{
		free(content);
		return (NULL);
	}
	new_elem->content = (void*)content;
	new_elem->next = NULL;
	new_elem->prev = NULL;
	return (new_elem);
}
