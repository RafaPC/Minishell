/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 11:18:22 by aiglesia          #+#    #+#             */
/*   Updated: 2021/03/14 12:34:24 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	not_preceeding_argument(char *input, int index)
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

char	**get_redirection_command(t_command_parsing *cmd_pars, char **input,
int index)
{
	int		lenght;
	char	**command_args;
	int		start;
	int		end;

	start = cmd_pars->i;
	cmd_pars->i = index;
	while (!(ft_isspace((*input)[cmd_pars->i]) ||
	ft_strchr("|<>;", (*input)[cmd_pars->i])))
	{
		if (ft_strchr("\"\'", (*input)[cmd_pars->i]))
			skip_quotations(input, cmd_pars);
		else if ((*input)[cmd_pars->i] == '\\')
			handle_backslash(input, cmd_pars->i, false);
		else
			cmd_pars->i++;
	}
	lenght = cmd_pars->i - index;
	end = index + lenght;
	if ((command_args = ft_calloc(2, sizeof(char *))))
		*command_args = ft_strncpy(&(*input)[index], lenght);
	cmd_pars->i = ft_extract(input, end - 1, end - start);
	cmd_pars->extracted_content = true;
	return (command_args);
}
