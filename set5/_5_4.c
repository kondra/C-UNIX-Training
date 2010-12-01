#include <stdio.h>
#include <math.h>

const int SIZE = 10;
const double EPS = 1e-8;

int nums[] = {1,2,3,4,5,6,7,8,9,10};
int tn;

typedef double (*func)(double);

int metric1(func f1, func f2, double param)
{
    if (f1(param) < f2(param) || fabs(f1(param) - f2(param)) < EPS)
        return -1;
    else return 1;
}

int metric2(func f1, func f2, double param)
{
    if (fabs(f1(param)) < fabs(f2(param)) || fabs(f1(param) - f2(param)) < EPS)
        return -1;
    else return 1;
}

void sort1(func *arr, int size, double user_param)
{
    int i, j;
    func tmp;
    for (i = 0; i < size; i++) {
        for (j = size - 1; j > i; j--) {
            if (metric1(arr[j - 1], arr[j], user_param) >= 0 && nums[j - 1] < nums[j]) {
                tmp = arr[j - 1];
                arr[j - 1] = arr[j];
                arr[j] = tmp;

                tn = nums[j - 1];
                nums[j - 1] = nums[j];
                nums[j] = tn;
            }
        }
    }
}

void sort2(func *arr, int size, double user_param)
{
    int i, j;
    func tmp;
    for (i = 0; i < size; i++) {
        for (j = size - 1; j > i; j--) {
            if (metric2(arr[j - 1], arr[j], user_param) >= 0 && nums[j - 1] > nums[j]) {
                tmp = arr[j - 1];
                arr[j - 1] = arr[j];
                arr[j] = tmp;

                tn = nums[j - 1];
                nums[j - 1] = nums[j];
                nums[j] = tn;
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

int main(void)
{
    FILE *f;
    f = freopen("input", "r", stdin);

    int i, k;
    double y, z;

    func arr[] = {sin, cos, func3, fabs, exp, func6, func7, func8, func9, func10};

    if (scanf("%d%lf%lf", &k, &y, &z) == EOF)
        return 0;

    if (k == 1)
        sort1 (arr, SIZE, y);
    else
        sort2 (arr, SIZE, y);

    for (i = 0; i < SIZE; i++)
        printf("%d ", nums[i]);
    printf("\n");

    for (i = SIZE - 1; i >= 0; i--)
        z = arr[i](z);

    printf("%.10lf\n", z);

    return 0;
}
