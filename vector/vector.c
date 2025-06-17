#include "vector/vector.h"
#include <math.h>

struct vector_3d vector_add(vector_3d_t a, vector_3d_t b)
{
    struct vector_3d v = {0};
    v.x = a->x + b->x;
    v.y = a->y + b->y;
    v.z = a->z + b->z;
    return v;
}

struct vector_3d vector_sub(vector_3d_t a, vector_3d_t b)
{
    struct vector_3d v = {0};
    v.x = a->x - b->x;
    v.y = a->y - b->y;
    v.z = a->z - b->z;
    return v;
}

struct vector_3d vector_cross(vector_3d_t a, vector_3d_t b)
{
    struct vector_3d v = {0};
    v.x = a->y * b->z - a->z * b->y;
    v.y = a->z * b->x - a->x * b->z;
    v.z = a->x * b->y - a->y * b->x;
    return v;
}

struct vector_3d vector_nabla(vector_time_row_t r)
{
    struct vector_3d v1 = {0};
    struct vector_3d v2 = {0};
    struct vector_3d w = {0};

    v2 = vector_sub(&r[2], &r[1]);
    v1 = vector_sub(&r[1], &r[0]);
    w = vector_sub(&v2, &v1);

    double dx = w.x, dy = w.y, dz = w.z;
    
    w.x = dz / (r[2].y - r[1].y) - dy / (r[2].z - r[1].z);
    w.y = dx / (r[2].z - r[1].z) - dz / (r[2].x - r[1].x);
    w.z = dy / (r[2].x - r[1].x) - dx / (r[2].y - r[1].y);

    return w;
}

struct vector_3d vector_rotation_x(vector_3d_t r, double phi_rad)
{
    struct vector_3d row1 = {1.0, 0.0, 0.0};
    struct vector_3d row2 = {0.0, cos(phi_rad), -sin(phi_rad)};
    struct vector_3d row3 = {0.0, sin(phi_rad), cos(phi_rad)};
    struct vector_3d v = {vector_dot(r, &row1), vector_dot(r, &row2), vector_dot(r, &row3)};
    return v;
}

struct vector_3d vector_rotation_y(vector_3d_t r, double phi_rad)
{
    struct vector_3d row1 = {cos(phi_rad), 0.0, sin(phi_rad)};
    struct vector_3d row2 = {0.0, 1.0, 0.0};
    struct vector_3d row3 = {-sin(phi_rad), 0.0, cos(phi_rad)};
    struct vector_3d v = {vector_dot(r, &row1), vector_dot(r, &row2), vector_dot(r, &row3)};
    return v;
}

struct vector_3d vector_rotation_z(vector_3d_t r, double phi_rad)
{
    struct vector_3d row1 = {cos(phi_rad), -sin(phi_rad), 0.0};
    struct vector_3d row2 = {sin(phi_rad), cos(phi_rad), 0.0};
    struct vector_3d row3 = {0.0, 0.0, 1.0};
    struct vector_3d v = {vector_dot(r, &row1), vector_dot(r, &row2), vector_dot(r, &row3)};
    return v;
}

struct vector_3d vector_multiply_scalar(vector_3d_t a, double s)
{
    struct vector_3d v = {0};
    v.x = a->x * s;
    v.y = a->y * s;
    v.z = a->z * s;
    return v;
}

struct vector_3d vector_divide_scalar(vector_3d_t a, double s)
{
    struct vector_3d v = {0};
    v.x = a->x / s;
    v.y = a->y / s;
    v.z = a->z / s;
    return v;
}

struct vector_astro vector_cartesian_to_astronomical(vector_3d_t a)
{
    struct vector_astro p = {0};
    const double rho_sqr = a->x * a->x + a->y * a->y;
    const double rho = sqrt(rho_sqr);
    p.r = sqrt(rho_sqr + a->z * a->z);
    p.theta = ((a->z == 0.0) || (rho == 0.0)) ? 0.0 : atan2(a->z, rho);
    p.phi = ((a->x == 0.0) || (a->y == 0.0)) ? 0.0 : atan2(a->y, a->x);
    p.phi = (p.phi >= 0.0) ? p.phi : p.phi + 2.0 * acos(-1.0);
    return p;
}

double vector_dot(vector_3d_t a, vector_3d_t b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

double vector_norm(vector_3d_t a)
{
    return sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
}

void vector_clear(vector_3d_t a)
{
    a->x = a->y = a->z = 0.0;
}
