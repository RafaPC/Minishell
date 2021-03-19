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

char    *handle_input(t_shell *shell) //Make shell global?
{
    shell->length = 0; //mem_set if we change it to a struct inside a struct?
    shell->line = ft_strdup("");
    shell->index = 0;
    shell->current_history = shell->history;
    set_terminal_mode(&shell->term, &shell->term_cp, 0);
    write_prompt();
    read_input(shell);
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
    }
    free (input);
}