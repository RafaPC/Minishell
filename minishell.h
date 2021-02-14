/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 00:21:15 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/14 10:53:15 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
/*
** Declares
*/
typedef struct			s_command
{
	char				**command;
	struct s_command	*next;
	int					relation;
}						t_command;

typedef struct 			split_commands
{
	int					i;
	int 				j;
	t_list				*arguments;
	int					error;
	int					error_index;
}              			t_command_parsing;

/*
**		EXECUTABLES PATHS
*/
int			execute_command(char **exec_paths, char *buffer);
char		**get_command_directories(char **exec_paths, char *command);
int			get_exec_paths_legnth(char **exec_paths);
char		**get_path(t_list *envp);
/*
**		READ INPUT
*/
void		tabs_to_spaces(char *string);
void		read_input(char *buffer);
void		empty_buffer(char *buffer);
int			insert_variable(char **input, int index);
int			split_commands(char **input, t_command	**commands);
void 		handle_quotations(char **input, t_command_parsing *cmd_pars);
void		add_command(t_command **commands, char **arguments, int relation);
t_bool		command_split(t_command_parsing *cmd_pars, t_command **commmands, char *input);
char 		**load_command_args(t_command_parsing *cmd_pars, char *input);
void		handle_redirections_split(t_command_parsing *cmd_pars,
			t_command **commands, char **input);

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

/* TEMPORARY */
char **get_false_env_list(void);
#endif