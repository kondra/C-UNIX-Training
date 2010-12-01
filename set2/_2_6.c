#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

const double eps = 0.1e-30;

int main(void)
{
    double x, frac, y;
    long long res, sign, floor_x;

    srand (time (NULL));
    while (1) {
        x = y = -(double) rand () / RAND_MAX;
        //    scanf ("%lf", &x);
        sign = x >= 0 ? 1 : -1;
        x = x >= 0 ? x : -x;

        floor_x = (long long) x;
        frac = x - floor_x - 0.5;
        if ((0.0 <= frac && frac <= eps) || (0.0 >= frac && frac >= -eps))
            res = sign > 0 ? floor_x + 1 : floor_x;
        if (frac > 0.0)
            res = floor_x + 1;
        if (frac < 0.0)
            res = floor_x;
        res *= sign;
        if (res != (long long) round (y)) {
            printf ("y = %lf\n", y);
            printf ("res = %lld\n", res);
            printf ("round = %lld\n", (long long) round (y));
            printf ("frac = %lf\n", frac);
            return 0;
        }
    }
 //   printf ("%d", res * sign);
    return 0;
}
