import basic # == from basic import *
from basic import test_func,item_one

def printme(str):
    "This prints a passed string into this function"
    print(str)
    return

printme("Hello world!")

#========================================arguments================================
def printinfo(name, age = 35):
    "This prints info passed in"
    print("Name: ", name, "; Age: ", age, sep='')
    return

#required
printinfo("Name", 50)

#keyword
printinfo(age=50, name="Name")

#default value
printinfo("Name")

#collections
def printlist(arg1, *list): # * indicates list
    "This prints arguments"
    print(arg1, end='\n')
    for var in list:
        print(var, end='\n')
    return

printlist("Names", "Mike", "Ben", "Name")

def printdict(arg1, **dict): # ** indicates dict
    "This prints arguments"
    print(arg1, end='\n')
    total = 0
    for var in dict: # returns list of keys
        print(var, ':', dict[var], sep=' ', end='\n')
        total += dict[var]
    return total

total = printdict("Inventory",
            ball=5.00,
            bat=15.00,
            glove=20.00)
print("Total:", total) # total not same variable as in function

#======================================lambda functions===================================
#lambda [arg1 [,arg2,....argn]]:expression
sum = lambda arg1, arg2: arg1 + arg2
print("Sum of 10 and 20:", sum(10, 20), sep=' ')

#=====================================modules========================
#basic.test_func()
#print("item_one from basic.py:", basic.item_one)