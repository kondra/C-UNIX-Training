#include <math.h>

const double EPS = 1e-10;

double min_value(double a, double b, double (*pf)(double))
{
    if (a > b) {
        double c = a;
        a = b;
        b = c;
    }

    double n1 = a + (b - a) / 3.0;
    double n2 = b - (b - a) / 3.0;
 
    if (fabs(b - a) < EPS)
        return a;

    double f1 = pf(n1);
    double f2 = pf(n2);

    if (f1 >= f2)
        return min_value(n1, b, pf);
    else
        return min_value(a, n2, pf);
}

#include <task.minf.c>
