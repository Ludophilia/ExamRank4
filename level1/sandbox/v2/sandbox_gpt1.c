#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	pid;
	int		status;

	if (f == NULL || timeout == 0)
		return (-1);

	pid = fork();
	if (pid < 0)
		return (-1);

	if (pid == 0)
	{
		alarm(timeout);
		f();
		exit(0);
	}

	if (waitpid(pid, &status, WUNTRACED) < 0)
		return (-1);

	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return (1);
		}

		if (verbose)
		{
			printf(
				"Bad function: exited with code %d\n",
				WEXITSTATUS(status)
			);
		}

		return (0);
	}

	if (WIFSIGNALED(status))
	{
		if (verbose)
		{
			if (WTERMSIG(status) == SIGALRM)
			{
				printf(
					"Bad function: timed out after %u seconds\n",
					timeout
				);
			}
			else
			{
				printf(
					"Bad function: %s\n",
					strsignal(WTERMSIG(status))
				);
			}
		}

		return (0);
	}

	if (WIFSTOPPED(status))
	{
		if (verbose)
		{
			printf(
				"Bad function: %s\n",
				strsignal(WSTOPSIG(status))
			);
		}

		kill(pid, SIGKILL);
		waitpid(pid, NULL, 0);

		return (0);
	}

	return (-1);
}
