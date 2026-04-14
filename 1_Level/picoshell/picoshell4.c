#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int    picoshell(char **cmds[])
{
	int fd[2];
	int prev_fd = -1;
	pid_t pid;
	for(int i = 0; cmds[i]; i++)
	{
		if (cmds[i + 1] && pipe(fd))
		{
			if (prev_fd != -1)
				close(prev_fd);
			break;
		}
		pid = fork();
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, 0);
				close(prev_fd);
			}
			if (cmds[i + 1])
			{
				dup2(fd[1], 1);
				close(fd[0]);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (cmds[i + 1])
		{
			prev_fd = fd[0];
			close(fd[1]);
		}
	}
	int w_status;
	int exit_status = 0;
	while (wait(&w_status) > 0)
	{
		if (WEXITSTATUS(w_status) == 1)
			exit_status = 1;
	}
	return (exit_status);
}
