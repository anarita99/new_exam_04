#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

/*int	picoshell(char **cmds[])
{
	pid_t	pid;
	pid_t	pid2;
	int		pipe_fds[2];

	pipe(pipe_fds);
	pid = fork();
	if(pid == 0)
	{
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);
		close(pipe_fds[0]);
		execvp(cmds[0][0], cmds[0]);
		return (0);
	}
	close(pipe_fds[1]);
	pid2 = fork();
	if(pid2 == 0)
	{
		dup2(pipe_fds[0], STDIN_FILENO);
		close(pipe_fds[0]);
		execvp(cmds[1][0], cmds[1]);
		return (0);
	}
	close(pipe_fds[0]);
	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (1);
}*/

int	picoshell(char **cmds[])
{
	pid_t*	pids;
	pid_t	pid_one;
	int		pipe_fds[2];
	int		i;
	int		prev_fd;
	int input_size;
	int w_status = 0;

	input_size = 0;
	while (cmds[input_size] != NULL)
		input_size++;   //saber o numero de comandos
	if(input_size == 1) //se for 1 so precisamos de dar fork, executar, sair com exit 1 na child
	{
		pid_one = fork();
		if(pid_one == 0)
		{
			execvp(cmds[0][0], cmds[0]);
			exit(1);
		}
		wait(&w_status); //no parent temos de usar o wait para esperar pela child e sair com o codigo certo
		return (w_status);
	}
	prev_fd = -1; //inicializamos o prev_fd com -1 para usar na condição a seguir
	i = 0;
	pids = malloc(sizeof(pid_t) * (input_size)); //damos malloc a todos os pids
	if(!pids)
		return (1);
	while (i < input_size) //serve para percorrer todos os comandos
	{
		if (i < input_size - 1) //se n estivermos no ultimo temos de criar o pipe
			pipe(pipe_fds);
		pids[i] = fork(); //damos fork para dar exec na child
		if(pids[i] == 0)
		{
			if(prev_fd != -1) //se n for o primeiro
			{
				dup2(prev_fd, STDIN_FILENO); //dizemos que o STDIN irá ser o comando de trás
				close(prev_fd);             //fechamos na child
			}
			if (i < input_size - 1)       //se n for o ultimo comando
			{
				dup2(pipe_fds[1], STDOUT_FILENO); //o STDOUT é enviado para o lado write do pipe
				close(pipe_fds[1]);               //fechamos os dois lados do pipe
				close(pipe_fds[0]);
			}
			execvp(cmds[i][0], cmds[i]);   //executamos o comando em si
			free(pids);                    //damos free aos pids e exit(1) se o exec n funcionar
			exit(1);
		}                     //no parent
		if(prev_fd != -1)    //se n for o primeiro comando podemos fechar o lado read do pipe
			close(prev_fd);
		if (i < input_size - 1)  //se n for o ultimo comando
		{
			prev_fd = pipe_fds[0]; //damos assign do prev ao read do pipe para usar em cima onde dizemos que o STDIN é o do pipe
			close(pipe_fds[1]);   //fechamos o write do pipe pq já nao o usamos
		}
		i++;
	}
	i = 0;
	while(i < input_size)  //percorremos todos os comandos
	{
		waitpid(pids[i], &w_status, 0); //esperamos por todas as child com o codigo do wait
		i++;
	}
	free(pids);   //damos free aos pids
	if (WIFEXITED(w_status)) //verifica se o processo terminou normalmente
		return WEXITSTATUS(w_status); //se sim  extrai o codigo de saida
	else if (WIFSIGNALED(w_status)) //verifica se foi terminado por um sinal 
		return (128 + WTERMSIG(w_status)); //retorna o numero do sinal que o matou 
	return (0);                    //Por que 128 + sinal? Quando um processo morre por sinal,
	                               // o exit code reportado é 128 + número_do_sinal.
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
