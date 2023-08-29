#include <numbers>
#include "calculationPower.h"



double calPos(double t, double f, double v) {
    //f*t/2 - 5*f/2 + 5*v0 + (5*f - 10*v0)*exp(-t/5)/2
    return f * t / 2 - 5 * f / 2 + 5 * v + (5 * f - 10 * v) * exp(-t / 5) / 2;
}

int func(const gsl_vector *x, void *params, gsl_vector *f) {
    double fx = reinterpret_cast<struct params *>(params)->fx;
    double fy = reinterpret_cast<struct params *>(params)->fy;
    double angle = reinterpret_cast<struct params *>(params)->angle;
    double dx = reinterpret_cast<struct params *>(params)->dx;
    double dy = reinterpret_cast<struct params *>(params)->dy;

    double force = gsl_vector_get(x, 0);
    double t = gsl_vector_get(x, 1);

    if (force < 0 || force > 100)
        return GSL_EBADFUNC;
    double vx = (force * cos(angle / 180 * std::numbers::pi) * 20);
    double vy = (force * sin(angle / 180 * std::numbers::pi) * 20);
    double f1 = calPos(t, fx, vx) - dx;
    double f2 = calPos(t, fy, vy) - dy;

    gsl_vector_set(f, 0, f1);
    gsl_vector_set(f, 1, f2);

    return GSL_SUCCESS;
}

std::optional<double> calForce(double wind, double angle, double dx, double dy) {
    gsl_multiroot_fsolver *s;
    int status;
    size_t iter = 0;

    const size_t n = 2;
    struct params p = {wind * 240, -7000, angle, dx * 100, dy * 100};
    gsl_multiroot_function f = {&func, n, &p};

    double x_init[2] = {50.0, 2.0};
    gsl_vector *x = gsl_vector_alloc(n);
    gsl_vector_set(x, 0, x_init[0]);
    gsl_vector_set(x, 1, x_init[1]);

    s = gsl_multiroot_fsolver_alloc(gsl_multiroot_fsolver_hybrids, n);
    gsl_multiroot_fsolver_set(s, &f, x);

    do {
        iter++;
        status = gsl_multiroot_fsolver_iterate(s);

        if (status) break;

        status = gsl_multiroot_test_residual(s->f, 1e-7);
    } while (status == GSL_CONTINUE && iter < 1000);
    if (status)
        return std::nullopt;
    double result = gsl_vector_get(s->x, 0);
    gsl_multiroot_fsolver_free(s);
    gsl_vector_free(x);
    return result;
}
