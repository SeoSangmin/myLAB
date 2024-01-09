#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#define BUFF_SIZE 36

void error(char *msg);
void child(int *fds);
void parent(pid_t child_process, int *fds);
int main()
{
	char buf[BUFF_SIZE];
	int read_byte = 0;
	int fds[2];
	pid_t pid;

	if (pipe(fds) == -1)
		error("pipe() error");

	if ((pid = fork()) == -1)
		error("fork() error");
	if (pid == 0)
		child(fds);
	else
		parent(pid, fds);
	return 0;
}

void child(int *fds)
{
	close(fds[0]);
	system("sleep 3");
	char *msg = "hello from child!";
	write(fds[1], msg, strlen(msg));
	return ;
}

void parent(pid_t child_process, int *fds)
{
	close(fds[1]);

	//fcntl(fds[0], F_SETFL, O_NONBLOCK);

	// read from pipe
	char buff[BUFF_SIZE];
	memset(buff, 0, sizeof(buff));
	int read_byte = read(fds[0], buff, BUFF_SIZE - 1);
	if (read_byte == -1)
	{
		if (errno == EAGAIN)
			printf("EAGAIN\n");
		else if (errno == EWOULDBLOCK)
			printf("EWOULDBLOCK\n");
		else
			printf("%s\n", strerror(errno));
	}
	else
	{
		buff[read_byte] = 0;
		printf("read : [%s]\n", buff);
	}

	system("sleep 5");
	read_byte = read(fds[0], buff, BUFF_SIZE - 1);
	if (read_byte > 0)
	{
		buff[read_byte] = 0;
		printf("second try : [%s]\n", buff);
	}
	close(fds[0]);
	waitpid(child_process, NULL, 0);
}

void error(char *msg)
{
	write(STDERR_FILENO, msg, strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	exit(1);
}
