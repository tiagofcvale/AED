#include <stdio.h>
#include <math.h>
#include <assert.h>

typedef struct {
    double x;
    double y;
} Ponto2D;

void pointInit(Ponto2D* p, double x, double y) {
    p->x = x;
    p->y = y;
}

double lenTo0(const Ponto2D* p) {
    return sqrt(p->x * p->x + p->y * p->y);
}
