/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdbl_move_backwards.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 00:37:25 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/20 00:37:38 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdbl_move_backwards(t_list_dbl *lst)
{
	void *aux;

	if (lst->next)
	{
		aux = lst->content;
		lst->content = lst->next->content;
		lst->next->content = aux;
	}
}
