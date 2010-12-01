#include <stdio.h>
#include <math.h>

const double eps = 0.1e-30;

int main(void)
{
    double y, x, frac;
    long long res, sign, floor_x;

    scanf ("%lf", &x);
    y = x;
    sign = x >= 0.0 ? 1 : -1;
    x = x >= 0.0 ? x : -x;

    floor_x = (long long) x;
    frac = x - floor_x - 0.5;
//    if ((0.0 <= frac && frac <= eps) || (0.0 >= frac && frac >= -eps))
    if (frac >= -eps && frac <= eps)    
        res = sign > 0 ? floor_x + 1 : floor_x;
    if (frac > 0.0)
        res = floor_x + 1;
    if (frac < 0.0)
        res = floor_x;

    printf ("%lld", res * sign);
    return 0;
}
