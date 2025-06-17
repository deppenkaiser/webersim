#pragma once

typedef struct vector_3d
{
    double x;
    double y;
    double z;
} *vector_3d_t;

typedef struct vector_astro
{
    double r;
    double phi;
    double theta;
} *vector_astro_t;

#define VECTOR_LOCATION_TIME_ROW_SIZE 3

typedef struct vector_3d vector_time_row_t[VECTOR_LOCATION_TIME_ROW_SIZE];

struct vector_3d vector_add(vector_3d_t a, vector_3d_t b);
struct vector_3d vector_sub(vector_3d_t a, vector_3d_t b);
struct vector_3d vector_cross(vector_3d_t a, vector_3d_t b);
struct vector_3d vector_nabla(vector_time_row_t r);
struct vector_3d vector_rotation_x(vector_3d_t r, double phi_rad);
struct vector_3d vector_rotation_y(vector_3d_t r, double phi_rad);
struct vector_3d vector_rotation_z(vector_3d_t r, double phi_rad);
struct vector_3d vector_multiply_scalar(vector_3d_t a, double s);
struct vector_3d vector_divide_scalar(vector_3d_t a, double s);
struct vector_astro vector_cartesian_to_astronomical(vector_3d_t a);
double vector_dot(vector_3d_t a, vector_3d_t b);
double vector_norm(vector_3d_t a);
void vector_clear(vector_3d_t a);
