/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdbl_delone.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 00:39:39 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/20 00:39:55 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdbl_delone(t_list_dbl *lst, void (*del)(void *))
{
	t_list_dbl *aux;

	if (del)
		del(lst->content);
	aux = lst;
	lst = lst->prev;
	if (lst)
		lst->next = aux->next;
	free(aux);
}
