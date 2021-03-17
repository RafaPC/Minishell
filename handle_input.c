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

void check_keys(int *index, int line_start)
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
    //if (i == 2)
        //copy temporal buffer to the actual buffer;
    if (buffer[0] == key_left && *index > line_start + 1)
    {
        ft_putstr_fd("\33[1D", STDIN_FILENO);
        (*index)--;
    }
    else if (buffer[0] == key_right)
    {
        ft_putstr_fd("\33[1C", STDIN_FILENO);
        (*index)++;
    }
}

void delete_char(int *index, int *length, int line_start)
{
    if (*index > line_start + 1)
    {
        ft_putstr_fd("\b\33[0K", STDIN_FILENO);
        //rewrite the rest...
        (*length)--;
        (*index)--;
    }
}

int write_prompt()
{
    char *str;

    str = "Minishell-> ";
    ft_putstr_fd("\033[32m", STDIN_FILENO);
    ft_putstr_fd(str, STDIN_FILENO);
    ft_putstr_fd("\033[0m", STDIN_FILENO);
    return(ft_strlen(str));
}

int main (void)
{
    struct termios term;
    struct termios term_save;
    char   buffer[1];
    int    index;
    int    length;
    int    line_start;
    
    length = 0;
    set_terminal_mode(&term, &term_save, 0);
    line_start = write_prompt();
    index = line_start + 1;
    while (read(STDIN_FILENO, buffer, 1))
    {
        if (buffer[0] == end_of_transmission)
            break ;
        else if (buffer[0] == delete)
            delete_char(&index, &length, line_start);
        else if (buffer[0] == escape)
            check_keys(&index, line_start);
        else
        {
            index += write(STDIN_FILENO, buffer, 1);
            length++;
        }
    }
    set_terminal_mode(&term, &term_save, 1);
}