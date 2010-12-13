#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char **argv)
{
    int msqid, max;

    key_t key;

    struct msgbuf {
        long type;
        long val;
    } buf;

    assert(argv[1]);
    max = strtol(argv[1], NULL, 10);
    key = ftok("../README", 0);

    msqid = msgget(key, 0666 | IPC_CREAT);

    buf.type = 1;
    buf.val = 0;

    printf("ping: maxval is %d\n", max);

    while (1) {
        msgsnd(msqid, &buf, sizeof(long), 0);
        msgrcv(msqid, &buf, sizeof(long), 2, 0);
        printf("ping: %ld\n", buf.val);
        buf.type = 1;
        buf.val++;
        if (buf.val > max) {
            printf("ping: maxval\n");
            buf.val = -1;
            msgsnd(msqid, &buf, sizeof(long), 0);
            msgrcv(msqid, &buf, sizeof(long), 3, 0);
            printf("ping: ok, deleting...\n");
            break;
        }
    }

    msgctl(msqid, IPC_RMID, 0);

    return 0;
}
