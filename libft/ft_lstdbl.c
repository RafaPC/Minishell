/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdbl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 09:59:03 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/18 10:51:47 by aiglesia         ###   ########.fr       */
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

void	ft_lstdbl_clear(t_list_dbl **lst, void (*del)(void *))
{
	t_list_dbl		*aux;

	if (lst == 0)
		return ;
	while ((*lst)->prev)
		*lst = (*lst)->prev;
	while (*lst)
	{
		aux = (*lst)->next;
		ft_lstdbl_delone(*lst, del);
		*lst = aux;
	}
}

void	ft_lstdbl_add_front(t_list_dbl **lst, t_list_dbl *new)
{
	new->next = *lst;
	if (*lst)
		(*lst)->prev = new;
	*lst = new;
}

t_list_dbl	*ft_lstdbl_last(t_list_dbl *lst)
{
	if (lst == 0)
		return (lst);
	else
		while (lst->next != 0)
			lst = lst->next;
	return (lst);
}

void	ft_lstdbl_add_back(t_list_dbl **lst, t_list_dbl *new)
{
	t_list_dbl	*p;

	p = *lst;
	if (!p)
		*lst = new;
	else
	{
		p = ft_lstdbl_last(p);
		p->next = new;
		p->next->prev = p;
	}
}

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

int		ft_lstdbl_size(t_list_dbl *lst)
{
	int	size;

	size = 0;
	while (lst)
	{
		lst = lst->next;
		size++;
	}
	return (size);
}