#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

static int	handle_status(int status, unsigned int timeout, bool verbose)
{
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return (1);
		}
		if (verbose)
			printf("Bad function: exited with code %d\n",
				WEXITSTATUS(status));
		return (0);
	}

	if (WIFSIGNALED(status))
	{
		if (verbose)
		{
			if (WTERMSIG(status) == SIGKILL)
				printf("Bad function: timed out after %u seconds\n",
					timeout);
			else
				printf("Bad function: %s\n",
					strsignal(WTERMSIG(status)));
		}
		return (0);
	}

	if (WIFSTOPPED(status))
	{
		if (verbose)
			printf("Bad function: %s\n",
				strsignal(WSTOPSIG(status)));
		return (0);
	}

	return (-1);
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	pid;
	int		status;
	time_t	start;

	if (f == NULL)
		return (-1);

	pid = fork();
	if (pid < 0)
		return (-1);

	if (pid == 0)
	{
		f();
		exit(0);
	}

	start = time(NULL);

	while (1)
	{
		/*
		** WNOHANG:
		**   don't block
		**
		** WUNTRACED:
		**   report stopped child too
		*/
		if (waitpid(pid, &status, WNOHANG | WUNTRACED) == -1)
			return (-1);

		/*
		** Child changed state
		*/
		if (
			WIFEXITED(status)
			|| WIFSIGNALED(status)
			|| WIFSTOPPED(status)
		)
			return (handle_status(status, timeout, verbose));

		/*
		** Timeout reached
		*/
		if ((unsigned int)(time(NULL) - start) >= timeout)
		{
			kill(pid, SIGKILL);

			/*
			** Reap child to avoid zombie
			*/
			if (waitpid(pid, &status, 0) == -1)
				return (-1);

			if (verbose)
				printf("Bad function: timed out after %u seconds\n",
					timeout);

			return (0);
		}

		/*
		** Avoid busy-looping CPU at 100%
		*/
		usleep(1000);
	}
}
