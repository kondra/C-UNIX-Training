#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int gate(const char *lock_name)
{
    int fd = open(lock_name, O_CREAT | O_EXCL, 0666);
    if (fd >= 1)
        return 0;
    if (errno == EEXIST)
        return 1;
    return -1;
}

int main(int argc, char *argv[])
{
    return 0;
}
