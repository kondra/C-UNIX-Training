#include <stdio.h>

const double eps = 0.1e-10;

int main (void)
{
    double a, b, c;
    scanf ("%lf%lf%lf", &a, &b, &c);

    if (a > eps || a < -eps) {
        double d = b * b - 4 * a * c;
        if (d >= 0.0 || ((0.0 <= d && d <= eps) || (0.0 >= d && d >= -eps))) {
            printf ("1");
            return 0;
        } else {
            printf ("0");
            return 0;
        }
    }
    if (b > eps || b < -eps) {
        printf ("1");
        return 0;
    }
    if (c > eps || c < -eps) {
        printf ("0");
    } else {
        printf ("1");
    }

    return 0;
}
