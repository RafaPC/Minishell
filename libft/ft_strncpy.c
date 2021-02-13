/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 10:52:50 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/13 11:12:14 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *s2, int n)
{
	char	*s;

	if (!s2)
		return(ft_strdup(""));
	if (!(s = malloc(n + 1)))
		return (0);
	ft_strlcpy(s, s2, n + 1);
	return (s);
}