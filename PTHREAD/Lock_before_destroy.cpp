#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cerrno>

typedef struct {
	pthread_mutex_t	*mutex;
	int				*share;
	int				idx;
} arguments;

void	*change_share_routine(void *arg);
void	*destroy_mutex_routine(void *arg);

int main(int argc, char **argv) {
	int				share = 0;
	pthread_t		tid[3];
	pthread_mutex_t	mutex;
	arguments		args;

	args.mutex = &mutex;
	args.share = &share;
	pthread_mutex_init(&mutex, NULL);

	/* pthread create */
	args.idx = 0;
	pthread_create(&tid[0], NULL, change_share_routine, (void*) &args);
	usleep(100);
	args.idx = 1;
	pthread_create(&tid[1], NULL, destroy_mutex_routine, (void*) &args);
	usleep(100);
	args.idx = 2;
	pthread_create(&tid[2], NULL, change_share_routine, (void*) &args);

	for (int i = 0; i < 3; ++i) {
		pthread_join(tid[i], NULL);
	}
	return 0;
}

void	*change_share_routine(void *arg) {
	arguments *args = (arguments*) arg;
	std::cout << "change_share_routine[" << args->idx << "]: ";
	if (pthread_mutex_lock(args->mutex) != 0) {
		std::cout << strerror(errno) << "\n";
		exit(1);
	}
	std::cout << "share : " << *(args->share) << " -> " << ++*(args->share) << "\n";
	pthread_mutex_unlock(args->mutex);
}

void	*destroy_mutex_routine(void *arg) {
	arguments *args = (arguments*) arg;
	std::cout << "destroy_mutex_routine[" << args->idx << "]: ";
	pthread_mutex_destroy(args->mutex);
	std::cout << "mutex destroyed\n";
	usleep(500);
	std::cout << "destroy_mutex_routine exit()\n";
}
