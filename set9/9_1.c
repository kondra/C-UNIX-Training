#include <stdio.h>

int main(void)
{
    int i, j, n, tmp, s;
    int a[2];
    FILE *out, *in;

    in = freopen("input.txt", "r", stdin);
    out = fopen("output.bin", "w+b");

    s = scanf("%d", &n);

    fseek(out, 0L, SEEK_SET);

    for (i = 0; i < n; i++) {
        s = scanf("%d", &tmp);
        s = fwrite(&tmp, sizeof (int), 1, out);
    }
    
    fflush(out);
    fseek(out, 0L, SEEK_SET);
    
    for (i = n - 1; i >= 0; i--) {
        for (j = 0; j < i; j++) {
            s = fread(a, sizeof (int), 2, out);
            if (a[0] > a[1]) {
                tmp = a[0];
                a[0] = a[1];
                a[1] = tmp;
                fseek(out, -2 * sizeof (int), SEEK_CUR);
                s = fwrite(a, sizeof (int), 2, out);
            }
            fseek(out, -sizeof (int), SEEK_CUR);
        }
        fseek(out, 0L, SEEK_SET);
    }

    fclose(in);
    fclose(out);

    return 0;
}

