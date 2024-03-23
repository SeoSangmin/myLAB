#include <stdio.h>
#include <pthread.h>
#define THREAD_NUM (5)

typedef struct _info {
	pthread_mutex_t *mutex_ptr;
	pthread_attr_t *attr_ptr;
	int value;
} info;

int sys_error(const char *err);
void *default_runner(void *args);
void *system_runner(void *args);
int main(int argc, char **argv) {
	pthread_t threads[THREAD_NUM];
	info share;
	share.value = 0;
	pthread_mutex_t mutex;
	pthread_attr_t attr;
	share.mutex_ptr = &mutex;
	share.attr_ptr = &attr;

	if (pthread_mutex_init(&mutex, NULL) != 0)
		return sys_error("pthread_mutex_init");
	if (pthread_attr_init(&attr) != 0)
		return sys_error("pthread_attr_init");

	int perio = 0;
	for (int i = 0; i < THREAD_NUM / 2; ++i) {
		pthread_setschedprio(threads[i], perio++);
		if (pthread_create(&threads[i], &attr, default_runner, (void *)&share) != 0)
			return sys_error("pthread_create");
	}
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	for (int i = THREAD_NUM / 2; i < THREAD_NUM; ++i) {
		if (pthread_create(&threads[i], &attr, system_runner, (void *)&share) != 0)
			return sys_error("pthread_create");
	}

	for (int i = 0; i < THREAD_NUM; ++i)
		pthread_join(threads[i], NULL);

	return 0;
}

void *default_runner(void *args) {
	info *share = (info *)args;
	pthread_mutex_lock(share->mutex_ptr);
	share->value += 1;
	printf("default runner value update(%d)\n", share->value);
	pthread_mutex_unlock(share->mutex_ptr);
	return NULL;
}

void *system_runner(void *args) {
	info *share = (info *)args;
	pthread_mutex_lock(share->mutex_ptr);
	share->value += 1;
	printf("system runner value update(%d)\n", share->value);
	pthread_mutex_unlock(share->mutex_ptr);
	return NULL;
}

int sys_error(const char *err) {
	printf("error :%s\n", err);
	return 1;
}
