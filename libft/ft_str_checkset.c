/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_checkchar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 11:57:54 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/07 10:35:03 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_bool	ft_str_checkset(char *string, char *set)
{
	if (string)
		while (*string)
		{
			if (ft_checkchar(*string, set))
				return (true);
			string++;
		}
	return (false);
}
