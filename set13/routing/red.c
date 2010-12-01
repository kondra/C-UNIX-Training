#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "red: wrong number of parameters\n");
        exit(-1);
    }

    int node, len, id, msg;
    int red, st, num, i, j;
    char *filename;
    FILE *f;

    red = strtol(argv[1], NULL, 10);
    filename = argv[2];

    f = freopen(filename, "r", stdin);
    if (f == NULL) {
        fprintf(stderr, "file %s permission denied\n", filename);
        exit(-1);
    }

    st = scanf("%d", &num);
    for (i = 0; i < num; i++) {
        st = scanf("%d%d%d", &id, &msg, &len);
        len--;

        assert(write(1, &id, sizeof (int)));
        assert(write(1, &msg, sizeof (int)));
        assert(write(1, &len, sizeof (int)));

        st = scanf("%d", &node);
        for (j = 1; j <= len; j++) {
            st = scanf("%d", &node);
            assert(write(1, &node, sizeof (int)));
        }
    }
    fprintf(stderr, "Red [%d]: %d packets transmitted\n", red, i);

    return 0;
}
