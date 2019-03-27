from math import *
import sys

def factorial(n):
    if (n <= 0):
        return 1
    else:
        return n * factorial(n - 1)

#def gammaFunction(x, n):
#    return (e ** -x) * (x ** (n - 1))
#
#def gamma(n):
#    if n % 1 == 0:
#        return factorial(n - 1)
#    else:
#        return areaUnderCurve(gammaFunction, 0, 535, n)

def areaUnderCurve(func_def, a, b, arg='', n=1000):
    dx = (b - a) / n
    area = val1 = val2 = 0
    for k in range(n):
        if arg:
            val1 = func_def(k * dx, arg)
            val2 = func_def((k + 1) * dx, arg)
        else:
            val1 = func_def(k * dx)
            val2 = func_def((k + 1) * dx)

        area += (dx / 2) * (val1 + val2)

    return area

def function(x):
    return x ** 4

def tpdf(t, v):
    return gamma((v + 1)/2) / (sqrt(v * pi) * gamma(v / 2)) * (1 + (t ** 2) / v) ** (-.5 * (v + 1))

def tcdf(lower, upper, v):
    return areaUnderCurve(tpdf, lower, upper, v)

print(tcdf(-1, 1, 15))