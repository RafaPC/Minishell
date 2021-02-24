/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_checkchar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 11:57:54 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/24 12:00:59 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_bool	ft_str_checkset(char *string, char *set)
{
	while (*string)
	{
		if (ft_checkchar(*string, set))
			return (true);
		string++;
	}
	return (false);
}
