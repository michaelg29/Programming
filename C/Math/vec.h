#include <stdarg.h>
#include <stdio.h>

typedef unsigned char bool;
#define true 1
#define false 0

#ifndef VEC_H
#define VEC_H

struct vec
{
    unsigned int dim;

    float *elements;
};
typedef struct vec vec;

extern const vec VEC_UNDEFINED;

vec allocate(unsigned int dim);

// default constructors
vec constructDefaultVector(unsigned int dim, float val);
vec constructEmptyVector(unsigned int dim);

// https://stackoverflow.com/questions/2124339/c-preprocessor-va-args-number-of-arguments
#define NUMARGS(...) (sizeof((float[]){0, ##__VA_ARGS__}) / sizeof(float) - 1)
#define newVector(...) vector(NUMARGS(__VA_ARGS__), ##__VA_ARGS__)

vec vector(unsigned int numArgs, ...);

vec copy(vec *v);

void print(vec v);

bool compare(vec v1, vec v2);

vec scalarMultiplication(vec v, float k);
void scalarMultiplicationBy(vec *v, float k);

vec scalarDivision(vec v, float k);
void scalarDivisionBy(vec *v, float k);

vec add(vec v1, vec v2);
bool addTo(vec *v1, vec v2);

vec subtract(vec v1, vec v2);
bool subtractFrom(vec *v1, vec v2);

vec multiply(vec v1, vec v2);
bool multiplyBy(vec *v1, vec v2);

vec divide(vec v1, vec v2);
bool divideBy(vec *v1, vec v2);

float dot(vec v1, vec v2);
bool orthogonal(vec v1, vec v2);

vec cross(vec v1, vec v2);

float magnitude(vec v);
float magnitudeSquared(vec v);
vec normalized(vec v);
void normalize(vec *v);

#endif