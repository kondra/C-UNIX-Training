#include <stdio.h>
#include <math.h>

const int SIZE = 10;
const double EPS = 1e-8;

typedef double (*func)(double);

void sort(func *arr, const func *arr_cmp, int size, int (*comp)(func, func, double, int, int), double user_param)
{
    int i, j, k, n1, n2;
    n1 = n2 = 0;
    func tmp;
    for (i = 0; i < size; i++) {
        for (j = size - 1; j > i; j--) {
            for (k = 0; k < size; k++) {
                if (arr[j - 1] == arr_cmp[k])
                    n1 = k + 1;
                if (arr[j] == arr_cmp[k])
                    n2 = k + 1;
            }
            if (comp(arr[j - 1], arr[j], user_param, n1, n2) < 0) {
                tmp = arr[j - 1];
                arr[j - 1] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}

double func3(double x)
{
    return x * x;
}

double func6(double x)
{
    return sqrt(42 + x * x);
}

double func7(double x)
{
    return x * x * x;
}

double func8(double x)
{
    return cos(x + M_PI / 3);
}

double func9(double x)
{
    return sin(x - M_PI / 42);
}

double func10(double x)
{
    return log(2 + x * x);
}

int metric1(func f1, func f2, double param, int n1, int n2)
{
    if (f1(param) < f2(param))
        return 1;
    if (fabs(f1(param) - f2(param)) < EPS && n1 < n2)
        return 1;
    return -1;
}

int metric2(func f1, func f2, double param, int n1, int n2)
{
    if (fabs(f1(param)) < fabs(f2(param)))
        return 1;
    if (fabs(f1(param) - f2(param)) < EPS && n1 > n2)
        return 1;
    return -1;
}

int main(void)
{
//    FILE *f;
//    f = freopen("input", "r", stdin);

    int i, k;
    double y, z;

    func arr[] = {sin, cos, func3, fabs, exp, func6, func7, func8, func9, func10};
    const func arr_cmp[] = {sin, cos, func3, fabs, exp, func6, func7, func8, func9, func10};

    if (scanf("%d%lf%lf", &k, &y, &z) == EOF)
        return 0;

    if (k == 1)
        sort (arr, arr_cmp, SIZE, metric1, y);
    else
        sort (arr, arr_cmp, SIZE, metric2, y);

    for (i = SIZE - 1; i >= 0; i--)
        z = arr[i](z);

    printf("%.10lf\n", z);

    return 0;
}
