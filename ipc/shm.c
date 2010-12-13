#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main(int argc, char **argv)
{
    int shmid, semid;
    int val = 0, maxval;
    int *shmaddr;

    struct sembuf P, V;
    V.sem_num = P.sem_num = 0;
    V.sem_flg = P.sem_flg = 0;

    assert(argv[1]);
    maxval = strtol(argv[1], NULL, 10);

    semid = semget(IPC_PRIVATE, 1, 0666);
    shmid = shmget(IPC_PRIVATE, sizeof(int), 0666);

    semctl(semid, 1, SETVAL, val);
    shmaddr = (int*) shmat(shmid, NULL, 0);

    if (fork() == 0) {
        printf("son\n");
        V.sem_op = P.sem_op = -1;
        while (1) {
            semop(semid, &P, 1);
            printf("son: %d\n", *shmaddr);
            (*shmaddr)++;
            semop(semid, &V, 1);
            if (*shmaddr == 0) {
                printf("son: terminating\n");
                exit(0);
            }
        }
    }

    *shmaddr = 0;
    V.sem_op = 2;
    P.sem_op = 0;
    printf("parent\n");
    while (1) {
        semop(semid, &P, 1);
        printf("parent: %d\n", *shmaddr);
        (*shmaddr)++;
        if (*shmaddr > maxval) {
            printf("parent: maxval\n");
            *shmaddr = -1;
            break;
        }
        semop(semid, &V, 1);
    }

    semop(semid, &V, 1);
    printf("parent: waiting\n");
    semop(semid, &P, 1);
    printf("parent: terminating\n");

    shmdt(shmaddr);
    shmctl(semid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}
