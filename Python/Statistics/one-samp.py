from math import *

def sum(vals):
    sum = 0
    
    for val in vals:
        sum += val

    return sum

def mean(vals):
    _sum = sum(vals)
    _length = len(vals)
    return _sum/_length

def variance(vals):
    _mean = mean(vals)
    _distances = []

    for val in vals:
        _distances.append(pow(val - _mean, 2))

    return sum(_distances) / len(_distances)

def stdDev(vals):
    return sqrt(variance(vals))

def min(vals):
    _min = vals[0]
    for i in range(1, len(vals)):
        if vals[i] < _min:
            _min = vals[i]

    return _min

def max(vals):
    _max = vals[0]
    for i in range(1, len(vals)):
        if vals[i] > _max:
            _max = vals[i]

    return _max

def median(vals):
    _len = len(vals)
    _med = vals[0]

    if _len % 2 == 0:
        _med = mean(vals[int(_len / 2 - 1):int(_len / 2 + 1)])
    else:
        _med = vals[int(_len / 2)]

    return _med

def q1(vals):
    _len = len(vals)
    _half = vals[:int(_len / 2)]
    return median(_half)

def q3(vals):
    _len = len(vals)
    _half = vals[int(_len / 2):]
    return median(_half)

vals = [500, 650, 600, 505, 450, 550, 515, 495, 650, 395]
vals.sort()
print(q3(vals))