#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double relu(double x);
double sigmoid(double x);

int main () {
    double (*activate)(double x) = relu;
    printf("%f\n", activate(-1.0));
    printf("%f\n", activate(2.0));

    activate = sigmoid;
    printf("%f\n", activate(0.0));
    return 0;
}

double relu(double x) {
    if (x > 0) {
        return x;
    }
    return 0;
}

double sigmoid(double x) {
    double e = exp(1.0);
    return 1 / (1 + pow(e, -x));
}