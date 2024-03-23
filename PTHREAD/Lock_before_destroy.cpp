#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <string>

#include "Colors.hpp"

typedef struct {
	pthread_mutex_t	*mutex;
	int				*share;
	int				idx;
} arguments;

void	*change_share_routine(void *arg);
void	*destroy_mutex_routine(void *arg);

int main(int argc, char **argv) {
	int				share = 0;
	pthread_t		tid[4];
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
	
	usleep(100);
	args.idx = 3;
	pthread_create(&tid[3], NULL, change_share_routine, (void*) &args);

	usleep(300);
	std::cout << BOLDGREEN << "\n\t\t\t\t\tthreads{ ";
	for (int i = 0; i < 4; ++i) {
		pthread_join(tid[i], NULL);
		std::cout << i << " ";
	}
	std::cout << "} have joined\n" << RESET;
	return 0;
}

void	*change_share_routine(void *arg) {
	arguments *args = (arguments*) arg;
	std::string name = (std::string)BLUE + "change_share_routine[" + std::to_string(args->idx) + "] " + RESET;
	std::string msg = name + " :wait\n";
	std::cout << msg;
	int ret = pthread_mutex_lock(args->mutex);
	std::cout << msg;
	if (ret == EINVAL) {
		msg = name + "lock -> " + (std::string)GREEN + "EINVAL :The value specified by mutex is invalid.\n" + RESET;
		std::cout << msg;
		return NULL;
	} else if (ret == EDEADLK) {
		msg = name + "lock -> " + (std::string)GREEN + "EDEADLK :A deadlock would occur if the thread blocked waiting for mutex.\n" + RESET;
		std::cout << msg;
		return NULL;
	} else if (ret == 0) {
		msg = name + "lock -> " + "success :: unlock the mutex and exit\n";
		std::cout << msg;
		pthread_mutex_unlock(args->mutex);
	}
}

void	*destroy_mutex_routine(void *arg) {
	arguments *args = (arguments*) arg;
	std::string name = (std::string)RED + "destroy_mutex_routine[" + std::to_string(args->idx) + "] " + RESET;
	std::string msg = name + " wait\n";
	std::cout << msg;
	// pthread_mutex_lock(args->mutex);
	// msg = name + "lock the mutex\n";
	std::cout << msg;
	pthread_mutex_destroy(args->mutex);
	msg = name + " mutex destroyed\n";
	std::cout << msg;

	usleep(300);
	// pthread_mutex_unlock(args->mutex);
	// msg = name + "unlock the mutex and exit\n";
	std::cout << msg;
}
