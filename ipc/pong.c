#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int msqid;

    key_t key;

    struct msgbuf {
        long type;
        long val;
    } buf;

    key = ftok("../README", 0);

    msqid = msgget(key, 0666);

    printf("pong: wait...\n");

    while (1) {
        msgrcv(msqid, &buf, sizeof(long), 1, 0);
        printf("pong: %ld\n", buf.val);
        if (buf.val == -1) {
            buf.type = 3;
            printf("pong: terminating\n");
            msgsnd(msqid, &buf, sizeof(long), 0);
            break;
        }
        buf.type = 2;
        buf.val++;
        msgsnd(msqid, &buf, sizeof(long), 0);
    }

    return 0;
}
