#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"

typedef enum	e_key_values
{
    end_of_transmission = 4,
    escape = 27,
    delete = 127,
    key_up = 'A',
    key_down = 'B',
    key_right = 'C',
    key_left = 'D'
}				t_key_values;

typedef struct          s_shell
{
	struct termios      term;
	struct termios      term_cp;
    char                *line;
	t_list_dbl          *history;
    t_list_dbl          *current_history;
    char                *h_saved_line;
    int                 index;
    int                 length;
}                       t_shell;

int write_prompt()
{
    char *str;

    str = "Minishell-> ";
    ft_putstr_fd("\033[32m", STDIN_FILENO);
    ft_putstr_fd(str, STDIN_FILENO);
    ft_putstr_fd("\033[0m", STDIN_FILENO);
    return(ft_strlen(str));
}

static int      set_terminal_mode(struct termios *term, struct termios *term_save, const int canonical)
 {
   char          *term_name;

    if (canonical == 0)
    {
       if ((term_name = getenv("TERM")) == NULL)
         return (-1);
        tcgetattr(STDIN_FILENO, term);
        tcgetattr(STDIN_FILENO, term_save);
        term->c_lflag &= ~(ICANON | ECHO);
        term->c_cc[VMIN] = 1;
        term->c_cc[VTIME] = 0;                                         
        tcsetattr(STDIN_FILENO, TCSANOW, term);
    }
   if (canonical == 1)
        tcsetattr(STDIN_FILENO, TCSANOW, term_save);
   return (0);
 }

 void   rewrite_line(t_shell *shell, char *line, t_bool erase_mode, t_bool duplicate_string)
 {
    if (erase_mode)
        free(shell->line);
    if (duplicate_string)
        shell->line = ft_strdup(shell->current_history->content);
    else
        shell->line = line;
    shell->index = ft_strlen(shell->line);
    shell->length = shell->index;
    ft_putstr_fd("\r\33[0J", STDIN_FILENO);
    write_prompt();
    ft_putstr_fd(shell->line, STDIN_FILENO);
 }

 void   delete_h_saved_line(t_shell *shell)
 {
    if (shell->h_saved_line)
    {
        free(shell->h_saved_line);
        shell->h_saved_line = 0;
    }
 }

void    retrieve_older_command(t_shell *shell)
{
    if (!shell->current_history)
        return ; //Maybe go all blinky blinky?
    if (!shell->current_history->prev && !shell->h_saved_line)
    {
        shell->h_saved_line = shell->line;
        rewrite_line(shell, shell->current_history->content, false, true);
        return ;
    }
    if (shell->current_history->next)
        shell->current_history = shell->current_history->next;
    rewrite_line(shell, shell->current_history->content, true, true);
}

void    retrieve_newer_command(t_shell *shell)
{
    if (!shell->current_history)
        return ; //Maybe go all blinky blinky?

    if (!shell->current_history->prev)
    {
        if (shell->h_saved_line)
        {
            rewrite_line(shell, shell->h_saved_line, false, false);
            shell->h_saved_line = 0;
        }
        return ;
    }
    if (shell->current_history->prev)
        shell->current_history = shell->current_history->prev;
    rewrite_line(shell, shell->current_history->content, true, true);
}

void check_keys(t_shell *shell)
{
    char keys[3];
    char buffer[1];
    int i = 1;

    ft_memset(&keys, 0, 3);
    keys[0] = 27;
    while (read(STDIN_FILENO, buffer, 1))
    {
        keys[i] = buffer[0];
        if (i == 1 && buffer[0] != '[')
            break;
        if (i == 2)
            break ;
        i++;
    }
    if (buffer[0] == key_left && shell->index)
    {
        ft_putstr_fd("\33[1D", STDIN_FILENO);
        (shell->index)--;
    }
    else if (buffer[0] == key_right && shell->index != shell->length)
    {
        ft_putstr_fd("\33[1C", STDIN_FILENO);
        (shell->index)++;
    }
    else if (buffer[0] == key_up)
        retrieve_older_command(shell);
    else if (buffer[0] == key_down)
        retrieve_newer_command(shell);
}

void set_cursor_back(char *written_sring)
{
    int xd;

    xd = ft_strlen(written_sring);
    while (xd--)            
        ft_putstr_fd("\b", STDIN_FILENO);
}

char    *delete_char(t_shell *shell)
{
    if (shell->index)
    {
        if (shell->index == shell->length)
        {
        ft_putstr_fd("\b\33[0K", STDIN_FILENO);
        (shell->index)--;
        (shell->length)--;
        ft_extract(&shell->line, shell->index, 1);
        ft_putstr_fd(&shell->line[shell->index], STDIN_FILENO);
        }
        else //Revise duplicities!
        {
            ft_putstr_fd("\b\33[0K", STDIN_FILENO);
            (shell->index)--;
            (shell->length)--;
            ft_extract(&shell->line, shell->index, 1);
            ft_putstr_fd(&shell->line[shell->index], STDIN_FILENO);
            set_cursor_back(&shell->line[shell->index]);
        }
        delete_h_saved_line(shell);
    }
    return (shell->line);
}

char    *handle_input(t_shell *shell) //Make shell global?
{
    char    buffer[1];
    
    delete_h_saved_line(shell);
    shell->length = 0; //mem_set if we change it to a struct inside a struct?
    shell->line = ft_strdup("");
    shell->index = 0;
    shell->current_history = shell->history;
    set_terminal_mode(&shell->term, &shell->term_cp, 0);
    write_prompt();
    while (read(STDIN_FILENO, buffer, 1))
    {
        if (buffer[0] == end_of_transmission || buffer[0] == '\n')
        {
            write(STDIN_FILENO, "\n", 1);
            break ;
        }
        else if (buffer[0] == escape)
            check_keys(shell);
        else if (buffer[0] == delete)
            shell->line = delete_char(shell);
        else
        {
            shell->index = ft_insert(&shell->line, buffer, shell->index, 1);
            ft_putstr_fd(&shell->line[shell->index - 1], STDIN_FILENO);
            set_cursor_back(&shell->line[shell->index]);
            shell->length++;
            delete_h_saved_line(shell);
        }
    }
    if (shell->line && *(shell->line) && (!shell->history ||
    ft_strncmp(shell->line, shell->history->content, shell->length)))
        ft_lstdbl_add_front(&shell->history, ft_lstdbl_new(ft_strdup(shell->line)));
    set_terminal_mode(&shell->term, &shell->term, 1);
    return (shell->line);
}

int main(void)
{
    t_shell shell;
    char    *input;
    t_list_dbl  *aux;

    ft_memset(&shell, 0, sizeof(t_shell));
    while (true)
    {
        input = handle_input(&shell);
        if (input && !ft_strncmp(input, "exit", 5))
            break ;
        else if (input && !ft_strncmp(input, "print", 6))
        {
            aux = shell.history;
            while (aux)
            {
                printf("%s\n", (char *)aux->content);
                aux = aux->next;
            }
        }
        else 
            printf("Input = %s\n", input);
        ft_putstr_fd("\a", STDIN_FILENO);
    }
    free (input);
}