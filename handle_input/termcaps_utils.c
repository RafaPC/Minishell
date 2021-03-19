/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 11:40:23 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/19 14:04:24 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct	s_action_codes
{
	char	*cursor_left;
	char	*cursor_right;
	char	*delete;
}				t_action_codes;
int tgetent (char *buffer, char *termtype);
char *tgetstr (char *name, char **area);

t_bool	get_termcap_codes(t_action_codes *action_codes)
{
	char *aux;
	if ((aux = tgetstr("le", NULL)))
		action_codes->cursor_left = aux;
	else
		return (false);
	if ((aux = tgetstr("nd", NULL)))
		action_codes->cursor_right = aux;
	else
		return (false);
	if ((aux = tgetstr("dc", NULL)))
		action_codes->delete = aux;
	else
		return (false);
}

t_bool	get_ansi_codes(t_action_codes *action_codes)
{
	char *aux;
	if (!(action_codes->cursor_left = ft_strdup("\27[1D")))
		return (false);
	if (!(action_codes->cursor_right = ft_strdup("\27[1C")))
		return (false);
	if (!(action_codes->delete = ft_strdup("\27[0K")))
		return (false);
	return (true);
}

void	free_action_codes(t_action_codes *action_codes)
{
	if (action_codes->cursor_left)
		free(action_codes->cursor_left);
	if (action_codes->cursor_right)
		free(action_codes->cursor_right);
	if (action_codes->delete)
		free(action_codes->delete);
}

t_bool	get_action_codes(t_action_codes *action_codes)
{
	char	*term_type;
	char	*termcap_description;
	t_bool	result;

	result = false;
	if (!(term_type = getenv("TERM")))
		return (NULL);
	if (tgetent(termcap_description, term_type) == 1)
		result = get_termcap_codes(action_codes);
	if (!result)
		get_ansi_codes(action_codes);
	if (term_type)
		free(term_type);
	if (termcap_description)
		free(termcap_description);
	if (result)
		free_action_codes(action_codes);
	return (result);
}
