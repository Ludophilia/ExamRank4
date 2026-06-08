#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int	picoshell(char ***gv)
{
	int	fd[2], pid, priv = 0, i = -1;

	while (gv[++i])
	{
		if (pipe(fd) < 0)
			return (-1);
		pid = fork();
		if (pid < 0)
			return (-1);
		if (pid == 0)
		{
			if (priv != 0)
			{
				dup2(priv, 0);
				close(priv);
			}
			if (gv[i + 1])
				dup2(fd[1], 1);
			close(fd[0]);
			close(fd[1]);
			execvp(gv[i][0], gv[i]);
		}
		if (priv > 0)
			close(priv);
		priv = fd[0];
		close(fd[1]);
	}
	while (wait(NULL) > 0);
	return (0);
}

int	main(void)
{
	char	***gv = (char **[]){(char *[]){"pwd", NULL}, (char *[]){"wc", "-c", NULL}, NULL};
	picoshell(gv);
	return (0);
}
