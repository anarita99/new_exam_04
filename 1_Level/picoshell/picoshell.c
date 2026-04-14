#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int	picoshell(char **cmds[])
{
	pid_t	pid;
	int		pipe_fds[2];
	int		prev_fd = -1;

	for (int i = 0; cmds[i]; i++)
	{
		if (cmds[i + 1] && pipe(pipe_fds))
		{
			if (prev_fd != -1)
				close(prev_fd);
			break;
		}
		pid = fork();
		if(pid == 0)
		{
			if(prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (cmds[i + 1])
			{
				dup2(pipe_fds[1], STDOUT_FILENO);
				close(pipe_fds[1]);
				close(pipe_fds[0]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if(prev_fd != -1)
			close(prev_fd);
		if (cmds[i + 1])
		{
			prev_fd = pipe_fds[0];
			close(pipe_fds[1]);
		}
	}
	int w_status;
	int exit_status = 0;
	while(wait(&w_status) > 0)
	{
		if (WEXITSTATUS(w_status) == 1)
			exit_status = 1;
	}
	return (exit_status);
}

/* int main(void)
{
	char **cmds[4];
	int		i;
	int		j;

	cmds[0] = malloc(sizeof(char *) * 3);
	cmds[1] = malloc(sizeof(char *) * 3);
	cmds[2] = malloc(sizeof(char *) * 3);

	cmds[0][0] = malloc(sizeof(char) * 3);
	cmds[0][1] = malloc(sizeof(char) * 3);
	cmds[0][2] = NULL;

	cmds[1][0] = malloc(sizeof(char) * 5);
	cmds[1][1] = malloc(sizeof(char) * 7);
	cmds[1][2] = NULL;

	cmds[2][0] = malloc(sizeof(char) * 4);
	cmds[2][1] = malloc(sizeof(char) * 3);
	cmds[2][2] = NULL;

	strcpy(cmds[0][0], "ls");
	strcpy(cmds[0][1], "-l");

	strcpy(cmds[1][0], "grep");
	strcpy(cmds[1][1], "main.c");

	strcpy(cmds[2][0], "cat");
	strcpy(cmds[2][1], "-e");

	cmds[3] = NULL;
	i = 0;
	picoshell(cmds);
	while(cmds[i])
	{
		j = 0;
		while(cmds[i][j])
		{
			free(cmds[i][j]);
			j++;
		}
		free(cmds[i]);
		i++;
	}
	
} */