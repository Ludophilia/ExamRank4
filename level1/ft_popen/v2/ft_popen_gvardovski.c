#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int ft_popen(char *file, char *gv[], char type)
{
	int fd[2], pid;

	if (!gv || !file || (type != 'r' && type != 'w'))
		return (-1);
	if (pipe(fd) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		if (type == 'r')
		{
			dup2(fd[1], 1);
			close(fd[0]);
			close(fd[1]);
		}
		if (type == 'w')
		{
			dup2(fd[0], 0);
			close(fd[0]);
			close(fd[1]);
		}
		execvp(file, gv);
	}
	if (type == 'r')
	{
		close(fd[1]);
		wait(NULL); // same, it won't work
		printf("ME? I WILL NEVER BE PRINTED. UNLESS ctrl + d.\n"); // ...
		return(fd[0]);
	}
	else
	{
		close(fd[0]);
		// printf("HELLO...\n"); // added for demo purposes
		wait(NULL); // jegerman: it freezes while in 'w' mode... Not good :/
		printf("ME? NEITHER.\n"); // ...
		return(fd[1]);
	}
}
