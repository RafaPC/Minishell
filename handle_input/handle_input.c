#include "minishell.h"

int write_prompt()
{
    char *str;

    str = "Minishell-> ";
    ft_putstr_fd("\033[32m", STDIN_FILENO);
    ft_putstr_fd(str, STDIN_FILENO);
    ft_putstr_fd("\033[0m", STDIN_FILENO);
    return(ft_strlen(str));
}

static int      set_terminal_mode(struct termios *term_save, const int canonical)
 {
   char          *term_name;
   struct        termios term;

    if (canonical == 0)
    {
       if ((term_name = getenv("TERM")) == NULL)
         return (-1);
        tcgetattr(STDIN_FILENO, &term);
        tcgetattr(STDIN_FILENO, term_save);
        term.c_lflag &= ~(ICANON | ECHO);
        term.c_cc[VMIN] = 1;
        term.c_cc[VTIME] = 0;                                         
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
    }
   if (canonical == 1)
        tcsetattr(STDIN_FILENO, TCSANOW, term_save); //handle un por si acaso
   return (0);
 }

t_bool  handle_input(char **buffer, t_list_dbl **command_history) //Make shell global?
{
    t_shell shell;

    ft_memset(&shell, 0, sizeof(t_shell));
    shell.line = ft_strdup("");
    shell.history = *command_history;
    shell.current_history = shell.history;
    set_terminal_mode(&shell.term_cp, 0);
    write_prompt();
    if (!read_input(&shell))
    {
        free (shell.line);
        set_terminal_mode(&shell.term_cp, 1);        
        return (handle_input(buffer, command_history));
    }
    set_terminal_mode(&shell.term_cp, 1);
    //handle memory stuff;
    *command_history = shell.history;
    *buffer = shell.line;
    return (true);
}
