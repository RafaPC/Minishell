/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 13:05:46 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/08 18:48:30 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (void)
{
	int filefd;
	int fd[2];
	char *str;

	pipe(fd);
	if (!fork())
	{
		close(fd[0]); // Close read since it wont be used;
		close(STDOUT_FILENO); //Close stdout
		dup2(fd[1], STDOUT_FILENO); //Using dup2, it makes sure that the filefd is set duplicated on STDOUT_FILENO
		printf("Hello world! im testing stuffs and things!");
	}
	else
	{
		close(fd[1]);
		wait(NULL);
		get_next_line(fd[0], &str); //Note that read waits for there to be something to read...
		printf("%s\n", str);
	}
	return 0;
}

