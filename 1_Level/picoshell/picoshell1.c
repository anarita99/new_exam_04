#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int	picoshell(char **cmds[])
{
	pid_t pid;
	int pipe_fds[2];
	int prev_fd = -1;

	for (int i = 0; cmds[i]; i++)
	{
		if (cmds[i + 1] && pipe(pipe_fds))
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
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (cmds[i + 1])
			{
				dup2(pipe_fds[1], STDOUT_FILENO);
				close(pipe_fds[0]);
				close(pipe_fds[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (cmds[i + 1])
		{
			prev_fd = pipe_fds[0];
			close(pipe_fds[1]);
		}
	}
	int exit_status = 0;
	int w_status;
	while (wait(&w_status) > 0)
	{
		if (WEXITSTATUS(w_status) == 1)
			exit_status = 1;
	}
	return exit_status;
}
