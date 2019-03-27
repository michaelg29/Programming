from math import *

x = 100.05
y = 5

abs(x)          # absolute value
fabs(x)         # absolute value
ceil(x)         # next integer
floor(x)        # previous integer
cmp(x, y)       # compare ----- -1 if x < y, 0 if x == y, or 1 if x > y
exp(x)          # e^x
log(x)          # natural logarithm of x = ln(x)
log10(x)        # log base 10 of x
max(x, y)       # returns max value
min(x, y)       # returns min value
modf(x)         # returns fractional and integral parts of x in a tuple
pow(x, y)       # x**y
round(x, y)     # x rounded to y digits ---- round(x, 0) = round(x)
sqrt(x)         # square root of x

#==============================random numbers====================
list = ["item1", 2, 3.0, False]

choice(list)        # random item from collection
randrange(0, 10, 2) # random item from range(0, 10, 2)
random()            # random float from 0 <= r < 1
seed(x)             # sets integer starting value in random
shuffle(list)       # shuffles list
uniform(x, y)       # random float x <= r < y

#===============================trigonometry=====================
acos(x)             # arc cosine in radians
asin(x)             # arc sine in radians
atan(x)             # arc tangent of x in radians
atan2(y, x)         # atan(y / x) in radians
cos(x)
sin(x)
tan(x)
hypot(x, y)         # sqrt(x**2 + y**2)
degrees(x)          # radians to degrees
radians(x)          # degrees to radians

#================================constants=======================
pi 
e
