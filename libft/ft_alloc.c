/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_alloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 11:09:30 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/04 11:41:01 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Allocates a memory segment of count * size + 1 and null terminates the result.
*/

void	*ft_alloc(size_t count, size_t size)
{
	char	*array;

	if (!(array = malloc(1 + count * size)))
		return (NULL);
	array[count * size] = 0;
	return ((void*)array);
}
