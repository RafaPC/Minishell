/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieto- <rprieto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 00:21:15 by rprieto-          #+#    #+#             */
/*   Updated: 2021/03/20 23:37:13 by rprieto-         ###   ########.fr       */
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
# include <termios.h>

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

typedef enum	e_key_values
{
	ctrl_c = 3,
	end_of_transmission = 4,
	escape = 27,
	delete = 127,
	left,
	right
}				t_key_values;

typedef struct	s_input_info
{
	struct termios		term_cp;
	char				*line;
	t_list_dbl			*history;
	t_list_dbl			*current_history;
	char				*h_saved_line;
	unsigned			index;
	unsigned			length;
}				t_input_info;

typedef struct	s_shell
{
	char		*buffer;
	t_list		*env_list;
	t_command	*commands;
	int			prev_exit_status;
	t_list_dbl	*command_history;
}				t_shell;
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
t_bool			handle_input(char **buffer, t_list_dbl **command_history);
t_bool			read_input(t_input_info *terminal);
void			handle_keys(t_input_info *terminal);
void			handle_input_history(t_input_info *terminal, char direction);
void			delete_h_saved_line(t_input_info *terminal);
int				write_prompt(void);
void			move_cursor(t_input_info *terminal, int direction,
t_bool change_index, unsigned nb);
void			delete_char(t_input_info *terminal);
void			handle_ctr_c_signal(t_input_info *terminal);
/*
**		COMMAND PARSING
*/
void			tabs_to_spaces(char *string);
void			empty_buffer(char *buffer);
int				insert_variable(char **input, int index, t_list *env_list,
int prev_exit_status);
int				split_commands(t_shell *shell);
void			handle_quotations(char **input, int *index, t_list *env_list,
int prev_exit_status);
void			skip_quotations(char *input, t_command_parsing *cmd_pars);
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
void			parse_insertions(t_shell *shell, t_bool single_run);
t_bool			not_preceeding_argument(char *input, int index);
int				handle_backslash(char **args, int index, t_bool remove);
char			**get_redirection_command(t_command_parsing *cmd_pars,
char **input, int index);
/*
**		MINISHELL UTILS
*/
t_list			*create_env_list(const char **env, char *command_path);
char			**env_list_to_array(t_list *env_list);
char			*get_env_var(char *var, t_list *env_list);
char			*get_command_path(char **paths, char *command);
t_command		*del_command(t_command *command);
void			free_commands(t_command *commands);
t_command		*handle_errors(t_command *command);
t_command		*print_redirection_errors(t_command *commands,
int *prev_exit_status);
void			debug_minishell(t_shell *shell, t_bool verbose);
/*
**		BUILTINS
*/
void			env(t_list *env_list, char *args);
void			echo(char **args, int i);
void			exit_command(t_shell *shell);
void			pwd(void);
void			cd(t_list **env_list, char **args);
void			free_and_exit(t_shell *shell, int exit_code);
/*
**				EXPORT
*/
void			export(t_list **env_list, char **args);
void			export_print(t_list *env_list);
t_bool			export_variable(t_list **env_list, char *arg);
t_bool			valid_env_characters(char *var_name);
/*
**				UNSET
*/
void			unset(t_list **env, char **args);
void			unset_variable(t_list **env, char *arg);
void			unset_recursive(t_list *env_list, t_list *previous_aux,
char *var_name, int compare_length);
/*
**		COMMAND EXECUTION
*/
void			child_process(t_shell *shell, t_bool is_file_path);
t_bool			is_directory(char *token, int *prev_exit_status);
t_bool			is_builtin(t_shell *shell, char *token);
t_bool			is_valid_path(char *path, int *prev_exit_status);
void			command_execution(t_shell *shell);
t_command		*execute_commands(t_shell *shell);
void			get_child_exit_status(int child_status, int *prev_exit_status);
void			restore_fds(int stdin_copy, int stdout_copy);
/*
**		COMMAND INPUT/OUTPUT
*/
t_bool			set_input_and_output(t_shell *shell, int mode);
t_bool			handle_pipe_and_execute(t_shell *shell);
/*
** TEMPORARY
*/
char			**get_false_env_array(void);
#endif
