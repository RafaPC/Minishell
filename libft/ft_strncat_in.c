/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 10:05:42 by user42            #+#    #+#             */
/*   Updated: 2020/10/22 18:06:50 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat_in(char *s1, char *s2, int n)
{
	size_t	dstsize;
	int		i;
	char	*s;

	i = 0;
	dstsize = ft_strlen(s1) + n + 1;
	if (!(s = malloc(dstsize)))
		return (0);
	if (!s1 && !s2)
		s[0] = 0;
	if (s1)
		i = ft_strlcpy(s, s1, ft_strlen(s1) + 1);
	if (s2)
		ft_strlcpy(s + i, s2, n + 1);
	return (s);
}
