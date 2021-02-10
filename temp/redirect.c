/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 13:05:46 by aiglesia          #+#    #+#             */
/*   Updated: 2021/02/08 18:22:47 by aiglesia         ###   ########.fr       */
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
	
	filefd = open("foo.txt", O_WRONLY|O_APPEND, 0666); //0666 = Permissions for the file.
	if (!fork()) {
	close(1);//Close stdout
	dup2(filefd, STDOUT_FILENO); //Using dup2, it makes sure that the filefd is set duplicated on STDOUT_FILENO
	//Ejecución del comando.
	} else {
	close(filefd);
	wait(NULL);
	}
	return 0;
}

