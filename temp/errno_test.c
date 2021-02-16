#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

void	signal_interrump(int signal)
{
	write(1, "\n", 1);
}

int main(int argc, char *const *argv, char *const *envp)
{
	int pid;
	int wstatus;

	signal(SIGINT, signal_interrump);
	if ((pid = fork()) == -1)
		printf("Error al forkear\n");
	if (pid == 0) // Hijo
	{
		if (execve("/usr/bin/find", argv, envp) == -1)
			exit(errno);
		else
			exit(0);
	}
	else // Padre
	{
		if (wait(&wstatus) == -1) // Error al esperar porque no haya hijos y cosas así
			printf("Error al esperar");
		else if (WIFEXITED(wstatus))
		{
			printf("Returned normally\n");
			printf("Code: %i\n", WEXITSTATUS(wstatus));
		}
		else if (WIFSIGNALED(wstatus))
		{
			printf("Returned by signal\n");
			printf("Signal code: %i\n", WTERMSIG(wstatus));
		}
		else
			printf("Not returned normally\n");
	}
	return (0);
}
