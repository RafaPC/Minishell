/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 00:21:15 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/11 16:25:56 by rprieto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>

/*
** Declares
*/

typedef enum	e_parser_flags
{
	simple_command,
	input_redirection,
	output_redirection,
	output_redirection_app,
	pipe_redirection,
	newline,
	semicolon
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
	t_bool				extracted_content;
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
int				insert_variable(char **input, int index, t_list *env_list, int prev_exit_status);
int				split_commands(char **input, t_command	**commands);
void			handle_quotations(char **input, t_command_parsing *cmd_pars, t_list *env_list, int prev_exit_status);
void			skip_quotations(char **input, t_command_parsing *cmd_pars);
void			add_command(t_command **commands, char **arguments,
					int relation);
t_bool			command_split(t_command_parsing *cmd_pars, t_command
					**commmands, char *input);
char			**load_command_args(t_command_parsing *cmd_pars, char *input);
void			handle_redirections_split(t_command_parsing *cmd_pars,
					t_command **commands, char **input);
void			handle_input_redirection(t_command_parsing *cmd_pars,
				t_command **commands, char **input);
int				print_parsing_error(int return_value, int *prev_exit_status);
void			parse_insertions(char **args, t_list *env_list, int prev_exit_status, t_bool single_run);
t_bool			not_preceeding_argument(char *input, int index);
/*
**		MINISHELL UTILS
*/
t_list			*create_env_list(const char **envp);
char			**env_list_to_array(t_list *env_list);
char			*get_env_var(char *var, t_list *env_list);
char			*get_command_path(char **paths, char *command);
t_command		*del_command(t_command *command);
t_command		*free_commands(t_command *commands);
t_command		*handle_errors(t_command *command);
t_command		*print_redirection_errors(t_command *commands, int *prev_exit_status);
void			debug_minishell(t_list **env_list, t_bool verbose);
/*
**		BUILTINS
*/
t_bool			env(t_list *env_list, char **args);
t_bool			echo(char **args);
void			ft_exit(t_command *commands, t_list **env_list);
t_bool			pwd(void);
t_bool			cd(t_list **env_list, char **args);
/*
**				EXPORT
*/
int				export(t_list **env_list, char **args);
int				export_print(t_list *env_list);
t_bool			export_variable(t_list **env_list, char *arg);
t_bool			valid_env_characters(char *var_name);
/*
**				UNSET
*/
int				unset(t_list **env_list, char **args);
t_bool			unset_recursive(t_list *env_list, t_list *previous_aux,
char *var_name, int compare_length);
/*
**		COMMAND EXECUTION
*/
void			child_process(t_command *command, t_bool is_file_path, t_list **env_list,
int *prev_exit_status);
t_bool			is_directory(char *token, int *prev_exit_status);
int				is_builtin(t_command *command, t_list **env_list, int *prev_exit_status);
void			wait_child_status(int *prev_exit_status);
t_bool			can_execute_file(char *path, int *prev_exit_status);
void			command_execution(t_command *command, t_list **env_list,
int *prev_exit_status);
t_command		*execute_commands(t_command *commands, t_list **env_list, int *prev_exit_status);
/*
**		COMMAND INPUT/OUTPUT
*/
int			get_input_and_output(
	char *file, int mode, int *prev_exit_status, t_list *env_list);
t_command	*set_fd(
	t_command *commands, t_list **env_list, int *prev_exit_status);
/*
** TEMPORARY
*/
char			**get_false_env_array(void);
#endif
