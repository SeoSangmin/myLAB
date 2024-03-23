#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024

int main() {
    int shmid;
    char *shm_ptr;

    // 공유 메모리 생성 또는 연결
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // 공유 메모리를 프로세스 주소 공간에 연결
    shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    // 공유 메모리에 데이터 쓰기
    char message[] = "Hello, shared memory!";
    strncpy(shm_ptr, message, SHM_SIZE);

    printf("Data written to shared memory: %s\n", shm_ptr);

    // 공유 메모리에서 데이터 읽기
    printf("Data read from shared memory: %s\n", shm_ptr);

    // 공유 메모리 분리
    if (shmdt(shm_ptr) == -1) {
        perror("shmdt");
        exit(1);
    }

    // 공유 메모리 삭제
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    return 0;
}
