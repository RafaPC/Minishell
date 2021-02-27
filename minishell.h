/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 00:21:15 by rprieto-          #+#    #+#             */
/*   Updated: 2021/02/27 20:06:38 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "ft_printf/ft_printf.h"
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>

/*
** Declares
*/

typedef enum 	e_parser_flags
{
		simple_command,
		input_redirection,
		output_redirection,
		output_redirection_app,
		pipe_redirection
}				t_parser_flags;

typedef struct	s_command
{
	char				**tokens;
	struct s_command	*next;
	int					relation;
}				t_command;

typedef struct	s_split_commands
{
	int					i;
	int					j;
	t_list				*arguments;
	int					error;
	int					error_index;
}				t_command_parsing;

/*
**		EXECUTABLES PATHS
*/
int				execute_command(char **exec_paths, char *buffer);
char			**get_command_directories(char **exec_paths, char *command);
int				get_exec_paths_legnth(char **exec_paths);
char			**get_path(t_list *env_list);
/*
**		READ INPUT
*/
void			tabs_to_spaces(char *string);
void			read_input(char **buffer);
void			empty_buffer(char *buffer);
int				insert_variable(char **input, int index, t_list *env_list);
int				split_commands(char **input, t_command	**commands, t_list *env_list);
void			handle_quotations(char **input, t_command_parsing *cmd_pars, t_list *env_list);
void			add_command(t_command **commands, char **arguments,
					int relation);
t_bool			command_split(t_command_parsing *cmd_pars, t_command
					**commmands, char *input);
char			**load_command_args(t_command_parsing *cmd_pars, char *input);
void			handle_redirections_split(t_command_parsing *cmd_pars,
					t_command **commands, char **input, t_list *env_list);
void			handle_input_redirection(t_command_parsing *cmd_pars,
				t_command **commands, char **input, t_list *env_list);
int				print_parsing_error(int return_value);

/*
**		MINISHELL UTILS
*/
t_list			*create_env_list(const char **envp);
char			**env_list_to_array(t_list *envp);
char			*get_env_var(char *var, t_list *envp);
char			*get_command_path(char **paths, char *command);
t_command		*del_command(t_command *command);
t_command		*free_commands(t_command *commands);
t_bool			handle_errors(t_command **command);
/*
**		BUILTINS
*/
t_bool			env(t_list *envp, char **args);
t_bool			echo(char **args);
void			ft_exit(t_command *commands, t_list **env_list, char **env_array);
t_bool			pwd(void);
t_bool			cd(char **args);
/*
**				EXPORT
*/
int				export(t_list **env, char **args);
int				export_print(t_list *envp);
t_bool			valid_env_characters(char *var_name);
/*
**				UNSET
*/
t_bool			unset(t_list **envp, char **args);
t_bool			unset_recursive(t_list *envp, t_list *previous_aux,
char *var_name, int compare_length);

/*
** TEMPORARY
*/
char			**get_false_env_array(void);
t_command		*execute_commands(t_command *commands, char ***env_array, t_list *env_list);
#endif
