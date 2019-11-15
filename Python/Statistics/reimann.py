import os

def func(x):
    return x ** 6

def get_int(prompt):
    while True:
        val = input(prompt)
        try:
            return int(val)
        except:
            print("Please input an integer.")

def get_int_range(prompt, lower, upper):
    while True:
        val = input(prompt)
        try:
            val = int(val)
            if val >= lower and val <= upper:
                return val
            else:
                print(f"Please input in range [{lower}, {upper}]")
        except:
            print("Please input an integer.")

def get_float_range(prompt, lower, upper):
    while True:
        val = input(prompt)
        try:
            val = float(val)
            if val >= lower and val <= upper:
                return val
            else:
                print(f"Please input in range [{lower}, {upper}]")
        except:
            print("Please input a float.")

while (True):
    print("REIMANN SUM CALCULATOR")
    print("----------------------")
    print("Ensure function input in def func(x)\n")

    a = get_int("Input left bound: ")
    b = get_int("Input right bound: ")
    n = get_int_range("Input number of rectangles: ", 1, 1e99)
    m = get_float_range("Input method (0 for LRAM, .5 for MRAM, 1 for RRAM): ", 0, 1)

    dx = (b - a) / n
    sum = 0

    for i in range(n):
        sum += dx * func(a + (i + m) * dx)

    print(f"Function evaluated from x = [{a}, {b}]")
    print(f"Used n = {n} rectangles")

    if m == 0:
        print("Left Reimann Sum = ", end="")
    elif m == .5:
        print("Midpoint Reimann Sum = ", end="")
    elif m == 1:
        print("Right Reimann Sum = ", end="")
    else:
        print("Random Reimann Sum = ", end="")

    print(sum)

    cont = input("Stop?\n")
    if cont == 'y' or cont == 'Y':
        break

    os.system("cls")
