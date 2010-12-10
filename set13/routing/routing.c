#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

//#define DEBUG

#ifdef DEBUG
#define debug(...) do { fprintf(stderr, "DEBUG (%d): ", getpid()); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr); } while (0)
#else
#define debug(...)
#endif

#define e_critical(...) do { fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE); } while (0)

enum {
    MAX_PATH_LEN = 100,
    MSG_ERROR = -1,
    MSG_END = 1,
    MSG_OK = 0,
    MSG_OFFSET = 3,
};

struct red_edge {
    int node;
    char *filename;
};

typedef struct {
    int r, b, m;

    int **vmatrix;
    int *write_num;
    int *read_num;
    struct red_edge *red;
} GraphData;

typedef struct Message {
    int ch, num_w;
    int sv_w, sv_r;
    int fd[2];
    int id, msg, len;
    int *node;
} Message;

typedef struct {
    int fd[2];
    int from, to;
} Pipe;

void set_nonblock(int fd)
{
    int fl;
    fl = fcntl(fd, F_GETFL, 0);
    assert(fl != -1);
    fl = fcntl(fd, F_SETFL, fl | O_NONBLOCK);
    assert(fl != -1);
}

void set_block(int fd)
{
    int fl;
    fl = fcntl(fd, F_GETFL, 0);
    assert(fl != -1);
    fl = fcntl(fd, F_SETFL, fl & ~O_NONBLOCK);
    assert(fl != -1);
}

GraphData *data_read(void)
{
    GraphData *data = NULL;

    int st, i, len;
    int **vmatrix, v1, v2;
    char buf[MAX_PATH_LEN], *ch;

    data = (GraphData*) malloc(sizeof(GraphData));
    assert(data != NULL);

    st = scanf("%d%d%d", &data->r, &data->b, &data->m);

    data->red = (struct red_edge*) malloc(sizeof(struct red_edge) * data->r);
    data->vmatrix = (int**) calloc(data->b, sizeof(int*));

    data->write_num = (int*) calloc(data->b, sizeof(int));
    data->read_num = (int*) calloc(data->b, sizeof(int));

    for (i = 0; i < data->b; i++) {
        data->vmatrix[i] = (int*) calloc(data->b, sizeof(int));
    }
    vmatrix = data->vmatrix;

    for (i = 0; i < data->r; i++) {
        st = scanf("%d", &data->red[i].node);
        data->red[i].node--;

        ch = fgets(buf, MAX_PATH_LEN, stdin);
        len = strlen(buf);
        data->red[i].filename = (char*) malloc(sizeof(char) * (len - 1));
        buf[len - 1] = '\0';
        strcpy(data->red[i].filename, buf + 1);
        data->red[i].filename[len - 1] = '\0';
    }

    for (i = 0; i < data->m; i++) {
        st = scanf("%d%d", &v1, &v2);
        v1--;
        v2--;
        vmatrix[v1][v2] = 1;
        data->write_num[v1]++;
        data->read_num[v2]++;
    }

    return data;
}

Message *message_read(int fd, int *status, Message *old)
{
    Message *msg = NULL;

    int st, t, i;

    if (old == NULL) {
        set_nonblock(fd);

        st = read(fd, &t, sizeof(int));

        if (st == 0) {
            if (status)
                *status = -1;
            return NULL;
        }

        if (st < 0) {
            if (errno == EAGAIN) {
                return NULL;
            } else {
                e_critical("something bad has happened while reading message, terminating...\n");
            }
        }

        msg = (Message*) malloc(sizeof(Message));
        msg->id = t;

        set_block(fd);

        st = read(fd, &msg->msg, sizeof(int));
        st = read(fd, &msg->len, sizeof(int));

        msg->node = (int*) malloc(sizeof(int) * msg->len);
        msg->sv_r = 1;
        msg->sv_w = -1;
        msg->ch = 0;

        msg->fd[0] = fd;
        msg->fd[1] = 0;

        if (msg->len > 0) {
            st = read(fd, &msg->node[0], sizeof(int));
        }
    } else {
        msg = old;
    }

    if (status == NULL) {
        set_nonblock(fd);
    }

    for (i = msg->sv_r; i < msg->len; i++) {
        st = read(fd, &msg->node[i], sizeof(int));
        if (st < 0 && errno == EAGAIN) {
            debug("fail %d: read", msg->id);
            msg->sv_r = i;
            return msg;
        }
    }
    msg->sv_r = msg->len;

    return msg;
}

int message_check(GraphData *data, int cur, Message *msg)
{
    msg->ch = 1;
    if (msg->len < 0 || (msg->len > 0 && (msg->node[0] <= 0 || msg->node[0] > data->b || data->vmatrix[cur][msg->node[0] - 1] == 0))) {
        printf("E:\t%d\t%d\t%d\t%d\n", getpid(), cur + 1, msg->id, msg->node[0]);
        return MSG_ERROR;
    }
    if (msg->len == 0) {
        printf("R:\t%d\t%d\t%d\t%d\n", getpid(), cur + 1, msg->id, msg->msg);
        return MSG_END;
    }
    printf("T:\t%d\t%d\t%d\t%d\n", getpid(), cur + 1, msg->id, msg->node[0]);
    return MSG_OK;
}

int message_send(int fd, Message *msg)
{
    int i, st;

    if (msg->sv_w == -1) {
        st = write(fd, &msg->id, sizeof(int));
        st = write(fd, &msg->msg, sizeof(int));
        msg->len--;
        st = write(fd, &msg->len, sizeof(int));
        msg->len++;
        msg->sv_w = 1;
        msg->fd[1] = fd;
    }

    for (i = msg->sv_w; i < msg->sv_r; i++) {
        st = write(fd, &msg->node[i], sizeof(int));
        if (st == -1 && errno == EAGAIN) {
            msg->sv_w = i;
            debug("fail %d: write", msg->id);
            return -1;
        }
    }
    msg->sv_w = i;
    if (i != msg->len) {
        debug("i!=len");
        return -1;
    }
    return 0;
}

void message_destroy(Message *msg)
{
    free(msg->node);
    free(msg);
}

void pipes_close(Pipe *p, int num)
{
    int i;
    for (i = 0; i < num; i++) {
        close(p[i].fd[0]);
        close(p[i].fd[1]);
    }
}

Pipe *pipes_create(int num)
{
    Pipe *tmp = NULL;

    int i, st;

    tmp = (Pipe*) malloc(sizeof(Pipe) * num);
    for (i = 0; i < num; i++) {
        st = pipe(tmp[i].fd);
        if (st == -1)
            e_critical("%d pipe failed\n", i);
    }

    return tmp;
}

Pipe **blue_pipe_create(GraphData *data)
{
    Pipe **bp = NULL;

    int **vmatrix = data->vmatrix;
    int i, j, k;

    bp = (Pipe**) malloc(sizeof(Pipe*) * data->b);
    for (i = 0; i < data->b; i++) {
        bp[i] = (Pipe*) malloc(sizeof(Pipe) * data->write_num[i]);
        bp[i] = pipes_create(data->write_num[i]);
        for (k = 0, j = 0; j < data->b; j++) {
            if (vmatrix[i][j] == 1) {
                bp[i][k].from = i;
                bp[i][k].to = j;
                k++;
            }
        }
    }

    return bp;
}

void close_pipe(int *sem)
{
    close(sem[0]);
    close(sem[1]);
}

int main(int argc, char **argv)
{
    if (argc < 3)
        e_critical("wrong number of parameters\n");

    GraphData *data;
    Pipe *red_pipe, **blue_pipe;

    int st, fail, i, j, r, b, **vmatrix, mark, red_balance, msg_balance;
    int sem[2], monster_pipe[2];
    pid_t *blue_pids;
    pid_t *red_pids;
    pid_t monster_pid, pid, rpid;

    const int mark_new_msg = 1;
    const int mark_dead_msg = -1;
    const int mark_dead_red = 0;

    data = data_read();

    b = data->b;
    r = data->r;
    vmatrix = data->vmatrix;

    red_pipe = pipes_create(r);
    blue_pipe = blue_pipe_create(data);

    blue_pids = (pid_t*) malloc(sizeof(pid_t) * b);

    //sync pipes
    if (pipe(sem))
        e_critical("first sync pipe failed\n");

    //blue processes
    for (i = 0; i < b; i++) {
        if ((pid = fork()) == 0) {
            Message *msg;
            Message **r_msg, **w_msg;
            Pipe *read_pipe;

            int *redp, redp_num, *isdead;
            int lock, l, k, s;
            int r_num;

            lock = 0;
            redp = (int*) malloc(sizeof(int) * r);
            isdead = (int*) malloc(sizeof(int) * r);
            //closing red pipes
            for (k = 0, j = 0; j < r; j++) {
                if (data->red[j].node == i) {
                    isdead[k] = 0;
                    redp[k++] = red_pipe[j].fd[0];
                    close(red_pipe[j].fd[1]);
                    continue;
                }
                close(red_pipe[j].fd[0]);
                close(red_pipe[j].fd[1]);
            }
            redp_num = k;

            //copy pipes from which we read for faster access
            //and close them for writing, also close other pipes which we don't use in this process
            read_pipe = (Pipe*) malloc(sizeof(Pipe) * data->read_num[i]);
            for (k = 0, j = 0; j < b; j++) {
                if (j == i)
                    continue;
                if (vmatrix[j][i] == 1) {
                    for (lock = -1, l = 0; l < data->write_num[j]; l++)
                        if (blue_pipe[j][l].to == i) {
                            read_pipe[k++] = blue_pipe[j][l];
                            lock = l;
                            break;
                        }
                }
                for (l = 0; l < data->write_num[j]; l++) {
                    if (l == lock) {
                        close(blue_pipe[j][l].fd[1]);
                        continue;
                    }
                    close(blue_pipe[j][l].fd[0]);
                    close(blue_pipe[j][l].fd[1]);
                }
            }

            r_msg = (Message**) calloc(data->read_num[i], sizeof(Message*));
            w_msg = (Message**) calloc(data->write_num[i], sizeof(Message*));

            //don't read from pipes in which we write
            for (j = 0; j < data->write_num[i]; j++)
                close(blue_pipe[i][j].fd[0]);

            //main loop
            while (1) {
                r_num = -1;
                msg = NULL;
                //try to read from red pipes
                for (s = 0, j = 0; j < redp_num; j++)
                    if (!isdead[j] && (msg = message_read(redp[j], &s, NULL)) != NULL) {
                        //if we've received something from red pipe
                        //we have a new packet in network
                        st = write(sem[1], &mark_new_msg, sizeof(int));
                        break;
                    } else if (s == -1) {
                        //eof in pipe, so red is dead, and we will not receive smth from this one
                        st = write(sem[1], &mark_dead_red, sizeof(int));
                        isdead[j] = 1;
                        s = 0;
                    }

                //try to read from blue pipes if nothing was read from red ones
                if (msg == NULL) {
                    for (j = 0; j < data->read_num[i]; j++)
                        if ((msg = message_read(read_pipe[j].fd[0], NULL, r_msg[j])) != NULL) {
                            if (msg->len > msg->sv_r) {
                                r_msg[j] = msg;
                                r_num = j;
                            } else {
                                r_msg[j] = NULL;
                            }
                            break;
                        }
                }

                //try again
                if (msg == NULL) {
                    for (j = 0; msg == NULL && j < data->read_num[i]; j++) {
                        if (r_msg[j] != NULL) {
                            msg = r_msg[j];
                            r_num = j;
                        }
                    }

                    for (j = 0; msg == NULL && j < data->write_num[i]; j++)
                        if (w_msg[j] != NULL)
                            msg = w_msg[j];

                    if (msg == NULL) {
                        usleep(100);
                        continue;
                    }
                }

                //check the message
                if (!msg->ch) {
                    st = message_check(data, i, msg);
                    fflush(stdout);
                    if (st == MSG_ERROR || st == MSG_END) {
                        //message is dead now
                        st = write(sem[1], &mark_dead_msg, sizeof(int));
                        message_destroy(msg);
                        continue;
                    }
                    //st == MSG_OK
                    //send message
                    for (j = 0; j < data->write_num[i]; j++) {
                        if (blue_pipe[i][j].to == msg->node[0] - 1) {
                            msg->num_w = j;
                            msg->fd[1] = blue_pipe[i][j].fd[1];
                            break;
                        }
                    }
                }

                st = message_send(msg->fd[1], msg);
                if (st == 0) {
                    message_destroy(msg);
                    w_msg[msg->num_w] = NULL;
                } else {
                    w_msg[msg->num_w] = msg;
                }
            }

            exit(EXIT_SUCCESS);
        } else if (pid == -1) {
            e_critical("failed on %d's blue fork\n", i);
        }

        blue_pids[i] = pid;
    }

    //close all blue pipes
    for (i = 0; i < b; i++) 
        pipes_close(blue_pipe[i], data->write_num[i]);

    //create red processes
    red_pids = (pid_t*) malloc(sizeof(pid_t) * r);
    for (i = 0; i < r; i++) {
        if ((pid = fork()) == 0) {
            char *filename = data->red[i].filename;
            char *name = argv[1];
            char num_buf[10];
            int pfd = red_pipe[i].fd[1];

            dup2(pfd, 1);
            pipes_close(red_pipe, r);
            close_pipe(sem);

            sprintf(num_buf, "%d", i + 1);
            for (j = strlen(name); j > 0 && name[j] != '/'; j--)
                ;

            execlp(name, name + j + 1, num_buf, filename, NULL);
            e_critical("%d red process exec failed\n", i);
        } else if (pid == -1) {
            e_critical("%d red process fork failed\n", i);
        }

        red_pids[i] = pid;
    }

    //close all red pipes
    pipes_close(red_pipe, r);

    //create monster pipe and monster process
    if (pipe(monster_pipe) < 0)
        e_critical("monster pipe failed\n");
    if ((monster_pid = fork()) == 0) {
        int *pfd = monster_pipe;
        char *name = argv[2];

        dup2(pfd[0], 0);
        close_pipe(sem);
        close_pipe(pfd);

        for (i = strlen(name); i > 0 && name[i] != '/'; i--)
            ;

        execlp(name, name + i + 1, NULL);
        e_critical("monster process exec failed\n");
    } else if (monster_pid == -1) {
        e_critical("monster process forking failed\n");
    }
    close(monster_pipe[0]);

    //wait red processes
    for (i = 0; i < r; i++) {
        rpid = wait(&st);

        //check if dead process was red
        //do we need this check or not?
        for (fail = 1, j = 0; j < r; j++)
            if (rpid == red_pids[j])
                fail = 0;
        if (fail) {
            e_critical("an error in blue process has occured, terminating...");
        }
    }

    //wait all packets to reach their destination
    //and all red pipes to be read
    red_balance = r;
    msg_balance = 0;

    while (red_balance || msg_balance) {
        st = read(sem[0], &mark, sizeof(int));
        if (st == 0)
            break;
        if (st == sizeof(int)) {
            if (mark == 0)
                red_balance--;
            else if (mark == 1)
                msg_balance++;
            else if (mark == -1)
                msg_balance--;
            continue;
        }
        e_critical("error in sync pipe\n");
    }

    //tell monster to kill our children
    st = write(monster_pipe[1], &b, sizeof(int));
    for (i = 0; i < b; i++) {
        st = write(monster_pipe[1], &blue_pids[i], sizeof(int));
    }
    close(monster_pipe[1]);

    //wait him to do his dirty work, ignoring our children deaths
    while (wait(&st) != monster_pid);

    //will close everything that was opened and free everything that was allocated
    return 0;
}
