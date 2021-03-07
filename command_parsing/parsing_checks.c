/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 11:18:22 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/07 11:41:22 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool not_preceeding_argument(char *input, int index)
{
	if (index == 0)
		return (true);
	else
		index--;
	while (index && ft_isspace(input[index]))
		index--;
	if (!index || ft_strrchr("|;", input[index]))
		return (true);
	return (false);
}