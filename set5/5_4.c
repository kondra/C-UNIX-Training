#include <stdio.h>
#include <math.h>

const int SIZE = 10;
const double EPS = 1e-8;

typedef double (*func)(double, int *);

void sort(func *arr, int size, int (*comp)(func, func, double), double user_param)
{
    int i, j;
    func tmp;
    for (i = 0; i < size; i++) {
        for (j = size - 1; j > i; j--) {
            if (comp(arr[j - 1], arr[j], user_param) >= 0) {
                tmp = arr[j - 1];
                arr[j - 1] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}

double func1(double x, int *n)
{
    if (n)
        *n = 1;
    return sin(x);
}

double func2(double x, int *n)
{
    if (n)
        *n = 2;
    return cos(x);
}

double func3(double x, int *n)
{
    if (n)
        *n = 3;
    return x * x;
}

double func4(double x, int *n)
{
    if (n)
        *n = 4;
    return fabs(x);
}

double func5(double x, int *n)
{
    if (n)
        *n = 5;
    return exp(x);
}
double func6(double x, int *n)
{
    if (n)
        *n = 6;
    return sqrt(42 + x * x);
}

double func7(double x, int *n)
{
    if (n)
        *n = 7;
    return x * x * x;
}

double func8(double x, int *n)
{
    if (n)
        *n = 8;
    return cos(x + M_PI / 3);
}

double func9(double x, int *n)
{
    if (n)
        *n = 9;
    return sin(x - M_PI / 42);
}

double func10(double x, int *n)
{
    if (n)
        *n = 10;
    return log(2 + x * x);
}

int metric1(func f1, func f2, double param)
{
    int n1, n2;
    f1(0, &n1);
    f2(0, &n2);
    if (n1 < n2) {
        if (f1(param, NULL) < f2(param, NULL) || fabs(f1(param, NULL) - f2(param, NULL)) < EPS)
            return -1;
    }
    return 1;
}

int metric2(func f1, func f2, double param)
{
    int n1, n2;
    f1(0, &n1);
    f2(0, &n2);
    if (n1 > n2) {
        if (fabs(f1(param, NULL)) < fabs(f2(param, NULL)) || fabs(f1(param, NULL) - f2(param, NULL)) < EPS)
            return -1;
    }
    return 1;
}

int main(void)
{
    FILE *f;
    f = freopen("input", "r", stdin);

    int i, k;
    double y, z;

    func arr[] = {func1, func2, func3, func4, func5, func6, func7, func8, func9, func10};

    if (scanf("%d%lf%lf", &k, &y, &z) == EOF)
        return 0;

    if (k == 1)
        sort (arr, SIZE, metric1, y);
    else
        sort (arr, SIZE, metric2, y);

    for (i = SIZE - 1; i >= 0; i--)
        z = arr[i](z, NULL);

    for (i = 0; i < SIZE; i++) {
        int j;
        arr[i](0, &j);
        printf("%d ", j);
    }
    printf("\n");

    printf("%.10lf\n", z);

    return 0;
}
