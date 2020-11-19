#include "vec.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern const vec VEC_UNDEFINED = {0, NULL};

vec allocate(unsigned int dim)
{
    vec ret;

    ret.dim = dim;
    ret.elements = malloc(dim * sizeof(float));

    return ret;
}

vec constructDefaultVector(unsigned int dim, float val)
{
    vec ret = allocate(dim);

    for (unsigned int i = 0; i < dim; i++)
    {
        ret.elements[i] = 0.0f;
    }

    return ret;
}

vec constructEmptyVector(unsigned int dim)
{
    constructDefaultVector(dim, 0.0f);
}

vec vector(unsigned int dim, ...)
{
    vec ret = allocate(dim);

    va_list list;
    va_start(list, dim);

    for (int i = 0; i < dim; i++)
    {
        ret.elements[i] = va_arg(list, double);
    }

    va_end(list);

    return ret;
}

vec copy(vec *v)
{
    vec ret;
    memcpy(&ret, v, sizeof(vec));
    return ret;
}

void print(vec v)
{
    printf("[ ");

    for (unsigned int i = 0; i < v.dim; i++)
    {
        printf("%f", v.elements[i]);

        if (i < v.dim - 1)
        {
            printf(", ");
        }
        else
        {
            printf(" ");
        }
    }

    printf("]\n");
}

bool compare(vec v1, vec v2)
{
    if (v1.dim == v2.dim)
    {
        for (unsigned int i = 0; i < v1.dim; i++)
        {
            if (v1.elements[i] != v2.elements[i])
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

vec scalarMultiplication(vec v, float k)
{
    vec ret = allocate(v.dim);

    for (unsigned int i = 0; i < ret.dim; i++)
    {
        ret.elements[i] = v.elements[i] * k;
    }

    return ret;
}
void scalarMultiplicationBy(vec *v, float k)
{
    for (unsigned int i = 0; i < v->dim; i++)
    {
        v->elements[i] *= k;
    }
}

vec scalarDivision(vec v, float k)
{
    vec ret = allocate(v.dim);

    for (unsigned int i = 0; i < ret.dim; i++)
    {
        ret.elements[i] = v.elements[i] / k;
    }

    return ret;
}
void scalarDivisionBy(vec *v, float k)
{
    for (unsigned int i = 0; i < v->dim; i++)
    {
        v->elements[i] /= k;
    }
}

vec add(vec v1, vec v2)
{
    if (v1.dim != v2.dim)
    {
        return VEC_UNDEFINED;
    }

    vec ret = allocate(v1.dim);

    for (unsigned int i = 0; i < ret.dim; i++)
    {
        ret.elements[i] = v1.elements[i] + v2.elements[i];
    }

    return ret;
}
bool addTo(vec *v1, vec v2)
{
    if (v1->dim != v2.dim)
    {
        return false;
    }

    for (unsigned int i = 0; i < v1->dim; i++)
    {
        v1->elements[i] += v2.elements[i];
    }

    return true;
}

vec subtract(vec v1, vec v2)
{
    if (v1.dim != v2.dim)
    {
        return VEC_UNDEFINED;
    }

    vec ret = allocate(v1.dim);

    for (unsigned int i = 0; i < ret.dim; i++)
    {
        ret.elements[i] = v1.elements[i] - v2.elements[i];
    }

    return ret;
}
bool subtractFrom(vec *v1, vec v2)
{
    if (v1->dim != v2.dim)
    {
        return false;
    }

    for (unsigned int i = 0; i < v1->dim; i++)
    {
        v1->elements[i] -= v2.elements[i];
    }

    return true;
}

vec multiply(vec v1, vec v2)
{
    if (v1.dim != v2.dim)
    {
        return VEC_UNDEFINED;
    }

    vec ret = allocate(v1.dim);

    for (unsigned int i = 0; i < ret.dim; i++)
    {
        ret.elements[i] = v1.elements[i] * v2.elements[i];
    }

    return ret;
}
bool multiplyBy(vec *v1, vec v2)
{
    if (v1->dim != v2.dim)
    {
        return false;
    }

    for (unsigned int i = 0; i < v1->dim; i++)
    {
        v1->elements[i] *= v2.elements[i];
    }

    return true;
}

vec divide(vec v1, vec v2)
{
    if (v1.dim != v2.dim)
    {
        return VEC_UNDEFINED;
    }

    vec ret = allocate(v1.dim);

    for (unsigned int i = 0; i < ret.dim; i++)
    {
        ret.elements[i] = v1.elements[i] / v2.elements[i];
    }

    return ret;
}
bool divideBy(vec *v1, vec v2)
{
    if (v1->dim != v2.dim)
    {
        return false;
    }

    for (unsigned int i = 0; i < v1->dim; i++)
    {
        v1->elements[i] /= v2.elements[i];
    }

    return true;
}

float dot(vec v1, vec v2)
{
    float ret = 0.0f;

    if (v1.dim == v2.dim)
    {
        for (unsigned int i = 0; i < v1.dim; i++)
        {
            ret += v1.elements[i] * v2.elements[i];
        }
    }

    return ret;
}

bool orthogonal(vec v1, vec v2)
{
    return v1.dim == v2.dim ? dot(v1, v2) == 0.0f : false;
}

vec cross(vec v1, vec v2)
{
    if (v1.dim != 3 || v2.dim != 3)
    {
        return VEC_UNDEFINED;
    }

    // formula from determinant
    vec ret = allocate(3);

    ret.elements[0] = (v1.elements[1] * v2.elements[2]) - (v1.elements[2] * v2.elements[1]);
    ret.elements[1] = -1 * ((v1.elements[0] * v2.elements[2]) - (v1.elements[2] * v2.elements[0]));
    ret.elements[2] = (v1.elements[0] * v2.elements[1]) - (v1.elements[1] * v2.elements[0]);

    return ret;
}

float magnitude(vec v)
{
    return sqrt(magnitudeSquared(v));
}

float magnitudeSquared(vec v)
{
    float ret = 0.0f;

    for (unsigned int i = 0; i < v.dim; i++)
    {
        ret += v.elements[i] * v.elements[i];
    }

    return ret;
}

vec normalized(vec v)
{
    vec ret = allocate(v.dim);
    float mag = magnitude(v);

    for (unsigned int i = 0; i < ret.dim; i++)
    {
        ret.elements[i] /= mag;
    }

    return ret;
}

void normalize(vec *v)
{
    float mag = magnitude(*v);

    for (unsigned int i = 0; i < v->dim; i++)
    {
        v->elements[i] /= mag;
    }
}