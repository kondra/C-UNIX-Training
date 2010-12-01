#include <stdio.h>

int main(void)
{
    long long a, b, c, d, e, n;
    scanf ("%lld%lld%lld%lld%lld%lld", &a, &b, &c, &d, &e, &n);
    long long x = (a + b) * c;
    long long y = d * e;
    if (x % n == y % n)
        printf ("YES");
    else
        printf ("NO");
    return 0;
}
