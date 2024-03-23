
// --------------------------------------------------------------//
// Suppot OS : Linux
// 			 - compile option '-pthread' must be included.
// --------------------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#define THREAD_NUM (5)
#define SEM "sem_pthread_test"

typedef struct _info {
    sem_t *sem;
	pthread_mutex_t *mutex_ptr;
	pthread_attr_t *attr_ptr;
	int *value;
    int id;
} info;

void sys_error(const char *err);
void *runner(void *args);
void sem_busy_wait(sem_t *sem);
int main(int argc, char **argv) {
    int shared_value = 0;
	pthread_t threads[THREAD_NUM];
	info shares[THREAD_NUM];

    pthread_mutex_t mutex;
    pthread_attr_t attr;
    sem_unlink(SEM);
    sem_t *sem = sem_open(SEM, O_CREAT, 0644, 0);
    if (sem == SEM_FAILED) {
        sem_close(sem);
        sys_error("sem_open");
    }

    for (int i = 0; i < THREAD_NUM; ++i) {
        shares[i].value = &shared_value;
        shares[i].mutex_ptr = &mutex;
        shares[i].attr_ptr = &attr;
        shares[i].sem = sem;
        shares[i].id = i;
    }
	
    if (pthread_mutex_init(&mutex, NULL) != 0)
		sys_error("pthread_mutex_init");
	if (pthread_attr_init(&attr) != 0)
		sys_error("pthread_attr_init");
    
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);

	for (int i = 0; i < THREAD_NUM; ++i) {
		if (pthread_create(&threads[i], &attr, runner, (void *)(&shares[i])) != 0)
			sys_error("pthread_create");
        pthread_setschedprio(threads[i], i);
	}

    sem_post(sem);
	for (int i = 0; i < THREAD_NUM; ++i)
		pthread_join(threads[i], NULL);

	return 0;
}

void *runner(void *args) {
    info *share = (info *)args;
    sem_busy_wait(share->sem);

	pthread_mutex_lock(share->mutex_ptr);
	*(share->value) += 1;
	printf("%d runner value update(%d)\n", share->id, *(share->value));
	pthread_mutex_unlock(share->mutex_ptr);
    sem_post(share->sem);
	return NULL;
}

void sem_busy_wait(sem_t *sem) {
    int ret = sem_trywait(sem);
    while (ret != 0) {
        usleep(10);
        ret = sem_trywait(sem);
        if (errno == (EDEADLK | EINTR | EINVAL))
            sys_error("sem)=_trywait");
    }
}

void sys_error(const char *err) {
	printf("error :%s\n", err);
	exit(1);
}
