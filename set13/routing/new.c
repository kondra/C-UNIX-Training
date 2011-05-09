#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

enum {
    MAXLEN = 100,
};

typedef struct Pipe {
    int fd[2];
    int to;
} Pipe;

int sem1[2], sem2[2];
int r, b, m;
int ball = 0;
Pipe *red, **blue;

void set_nonblock(int fd)
{
    int fl;
    fl = fcntl(fd, F_GETFL, 0);
    fl = fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}

void set_block(int fd)
{
    int fl;
    fl = fcntl(fd, F_GETFL, 0);
    fl = fcntl(fd, F_SETFL, fl & ~O_NONBLOCK);
}

void close_pipe(int *fd)
{
    close(fd[0]);
    close(fd[1]);
}

void close_all(void)
{
    int j, k;
    for (j = 0; j < r; j++) {
        close_pipe(red[j].fd);
    }
    for (j = 0; j < b; j++) {
        for (k = 0; k < b; k++) {
            if (blue[j][k].to != -1) {
                close_pipe(blue[j][k].fd);
            }
        }
    }
}

void process(int fd, int cur, int isred)
{
    int id, msg, len;
    int *nodes, fd2, st;
    set_nonblock(fd);
    st = read(fd, &id, sizeof(int));
    if (st == -1 && errno == EAGAIN) {
        return;
    }
    if (st == 0 && isred > -1) {
        while (read(sem1[0], &ball, sizeof(int)) != sizeof(int));
        red[isred].to = -1;
        return;
    }
    set_block(fd);
    if (isred > -1) {
        st = write(sem2[1], &ball, sizeof(int));
    }
    st = read(fd, &msg, sizeof(int));
    st = read(fd, &len, sizeof(int));
    if (len == 0) {
        while (read(sem2[0], &ball, sizeof(int)) != sizeof(int));
        printf("R:\t%d\t%d\t%d\t%d\n", getpid(), cur, id, msg);
        return;
    }
    nodes = (int*) malloc(sizeof(int) * len);
    st = read(fd, nodes, sizeof(int) * len);
    if (nodes[0] > b || nodes[0] <= 0 || blue[cur][nodes[0] - 1].to == -1) {
        while (read(sem2[0], &ball, sizeof(int)) != sizeof(int));
        printf("E:\t%d\t%d\t%d\t%d\n", getpid(), cur, id, nodes[0]);
        free(nodes);
        return;
    }
    len--;
    fd2 = blue[cur][nodes[0] - 1].fd[1];
    st = write(fd2, &id, sizeof(int));
    st = write(fd2, &msg, sizeof(int));
    st = write(fd2, &len, sizeof(int));
    st = write(fd2, nodes + 1, sizeof(int) * len);
    printf("T:\t%d\t%d\t%d\t%d\n", getpid(), cur, id, nodes[0]);
    free(nodes);
}

int main(int argc, char **argv)
{
    char **filepath, num[10];
    int monster[2], st, *pids;
    int i, j, k, to, from;

    st = scanf("%d%d%d", &r, &b, &m);

    red = (Pipe*) malloc(sizeof(Pipe) * r);
    filepath = (char**) malloc(sizeof(char*) * r);
    for (i = 0; i < r; i++) {
        filepath[i] = (char*) malloc(sizeof(char) * MAXLEN);
        st = scanf("%d %s\n", &to, filepath[i]);
        st = pipe(red[i].fd);
        to--;
        red[i].to = to;
    }

    blue = (Pipe**) malloc(sizeof(Pipe*) * b);
    for (i = 0; i < b; i++) {
        blue[i] = (Pipe*) malloc(sizeof(Pipe) * b);
    }
    for (i = 0; i < b; i++) {
        for (j = 0; j < b; j++) {
            blue[i][j].to = -1;
        }
    }
    for (i = 0; i < m; i++) {
        st = scanf("%d%d", &from, &to);
        from--;
        to--;
        blue[from][to].to = to;
        st = pipe(blue[from][to].fd);
    }

    st = pipe(sem1);
    st = pipe(sem2);
    set_nonblock(sem1[0]);
    set_nonblock(sem2[0]);
    pids = (int*) malloc(sizeof(int) * b);
    for (i = 0; i < r; i++) {
        st = write(sem1[1], &ball, sizeof(int));
    }

    for (i = 0; i < b; i++) {
        if ((pids[i] = fork()) == 0) {
            for (j = 0; j < b; j++) {
                for (k = 0; k < b; k++) {
                    if (blue[j][k].to == -1) {
                        continue;
                    }
                    if (j == i) {
                        close(blue[j][k].fd[0]);
                        continue;
                    }
                    if (k == i) {
                        close(blue[j][k].fd[1]);
                        continue;
                    }
                    close_pipe(blue[j][k].fd);
                }
            }

            for (j = 0; j < r; j++) {
                if (red[j].to == i) {
                    close(red[j].fd[1]);
                } else {
                    close_pipe(red[j].fd);
                }
            }

            while (1) {
                for (j = 0; j < r; j++) {
                    if (red[j].to == i) {
                        process(red[j].fd[0], i, j);
                    }
                }
                for (j = 0; j < b; j++) {
                    if (blue[j][i].to != -1) {
                        process(blue[j][i].fd[0], i, -1);
                    }
                }
            }
        }
    }

    for (i = 0; i < r; i++) {
        if (fork() == 0) {
            close_pipe(sem1);
            close_pipe(sem2);
            dup2(red[i].fd[1], 1);
            close_all();
            sprintf(num, "%d", i);
            execlp(argv[1], argv[1], num, filepath[i], NULL);
            exit(0);
        }
    }
    close_all();
    for (i = 0; i < r; i++) {
        wait(&st);
    }

    st = pipe(monster);
    if (fork() == 0) {
        close_pipe(sem1);
        close_pipe(sem2);
        dup2(monster[0], 0);
        close_pipe(monster);
        execlp(argv[2], argv[2], NULL);
        exit(0);
    }

    while (1) {
        st = read(sem1[0], &ball, sizeof(int));
        if (st < 0 && errno == EAGAIN) {
            break;
        }
        st = write(sem1[1], &ball, sizeof(int));
    }
    while (1) {
        st = read(sem2[0], &ball, sizeof(int));
        if (st < 0 && errno == EAGAIN) {
            break;
        }
        st = write(sem2[1], &ball, sizeof(int));
    }

    st = write(monster[1], &b, sizeof(int));
    for (i = 0; i < b; i++) {
        st = write(monster[1], &pids[i], sizeof(int));
    }
    for (i = 0; i < b; i++) {
        wait(&st);
    }
    wait(&st);

    return 0;
}
