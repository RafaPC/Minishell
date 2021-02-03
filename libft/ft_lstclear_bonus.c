/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 12:38:47 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/03 13:18:01 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list		*aux;

	if (lst == 0)
		return ;
	while (*lst)
	{
		aux = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = aux;
	}
}
