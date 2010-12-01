#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#define READ 4
#define WRITE 2
#define EXEC 1
#define U_SHIFT 6
#define G_SHIFT 3

void print_rights(int mode)
{
    if (READ & mode) printf("r");
    else printf("-");
    if (WRITE & mode) printf("w");
    else printf("-");
    if (EXEC & mode) printf("x");
    else printf("-");
}

int main(int argc, char *argv[])
{
    int owner, gr, others;
    struct stat st;
    stat(argv[1], &st);
    owner = st.st_mode & S_IRWXU;
    gr = st.st_mode & S_IRWXG;
    others = st.st_mode & S_IRWXO;
    print_rights(owner >> U_SHIFT);
    print_rights(gr >> G_SHIFT);
    print_rights(others);
    return 0;
}
