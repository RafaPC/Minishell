/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 00:21:15 by rprieto-          #+#    #+#             */
/*   Updated: 2021/01/25 17:40:12 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"
int		execute_command(char **exec_paths, char *buffer);
/*
**		EXECUTABLES PATHS
*/
char	**get_command_directories(char **exec_paths, char *command);
int		get_exec_paths_legnth(char **exec_paths);
char	**get_path(t_list *envp);
/*
**		READ INPUT
*/
void	tabs_to_spaces(char *string);
void	read_input(char *buffer);
void	empty_buffer(char *buffer);
/*
**		BUILTINS
*/
t_bool	export(t_list *envp, char *arg);
void	unset(t_list *envp, char *var_name);
t_list	*get_env_list(const char **envp);
#endif