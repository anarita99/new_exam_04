#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int ft_popen(const char *file, char *const argv[], char type)
{
	int fd[2];
	pid_t pid;
	
	if (!file || !argv || (type != 'r' && type != 'w') || pipe(fd) < 0)
		return -1;
	if ((pid = fork()) < 0)
	{
		close(fd[0]);
		close(fd[1]);
		return -1;
	}
	if (pid == 0)
	{
		dup2(fd[type == 'r' ? 1 : 0], type == 'r' ? 1 : 0);
		close(fd[0]);
		close(fd[1]);
		execvp(file, argv);
		exit(1);
	}
	close(fd[type == 'r' ? 1 : 0]);
	return fd[type == 'r' ? 0 : 1];
}

/*dup2(fd[type == 'r' ? 1 : 0], type == 'r' ? 1 : 0);
if (type == 'r')
	dup2(fd[1], 1);
else
	dup2(fd[0], 0);
}*/