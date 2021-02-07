/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 00:21:15 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/07 14:04:18 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"
/*
**		EXECUTABLES PATHS
*/
int		execute_command(char **exec_paths, char *buffer);
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
**		MINISHELL UTILS
*/
t_list	*create_env_list(const char **envp);
char	**env_list_to_char(t_list *envp);
char	*get_env_var(char *var, t_list *envp);
/*
**		BUILTINS
*/
char	*env(t_list *envp, t_list *envp_copy, int length, int i);
void	ft_exit(t_list **envp);
/*
**				EXPORT
*/
char	*export(t_list *envp, char *arg);
char	*export_print(t_list *envp, char *return_string);
char	*get_env_string_pointer(t_list *envp);
/*
**				UNSET
*/
void	unset(t_list **envp, char *var_name);
void	unset_recursive(t_list *envp, t_list *previous_aux,
char *var_name, int compare_length);
#endif