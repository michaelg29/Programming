#exec(open(r"C:\Users\Michael Grieco\Documents\Python\first.py").read())

#==================================comments==============================================

#single line

'''
multi-line comments
'''

print('Hello world!')

#=================================indentation============================================
if 3 > 5:
    print('3 > 5')
else:
    print('3 < 5')

#throws error:

'''
if 3 > 5:
print('3 > 5')
else:
print('3 < 5')
'''

#======================================declarations=========================================
item_one = 15

#multiple statements on one line
item_two = 30; item_three = 14.44

#one statement on multiple lines
total = item_one + \
        item_two + \
        item_three

#multiple assignment
a = b = c = 1
a,b,c = 1,2,"John"

del a
del b, c

#quotations
string = 'hi'
string = "hi"
string = """hi"""

#=========================================numbers===========================================
#int
a = 1
#long
#b = 51924361L
#float
c = 0.0
#complex
d = -.6545 + 5J

print(a,c,d)

#========================================conversion==========================================
'''	
int(x [,base])          - Converts x to an integer. base specifies the base if x is a string.
long(x [,base] )        - Converts x to a long integer. base specifies the base if x is a string.
float(x)                - Converts x to a floating-point number.
complex(real [,imag])   - Creates a complex number.
str(x)                  - Converts object x to a string representation.
repr(x)                 - Converts object x to an expression string.
eval(str)               - Evaluates a string and returns an object.
tuple(s)                - Converts s to a tuple.
list(s)                 - Converts s to a list.
set(s)                  - Converts s to a set.
dict(d)                 - Creates a dictionary. d must be a sequence of (key,value) tuples.
frozenset(s)            - Converts s to a frozen set.
chr(x)                  - Converts an integer to a character.
unichr(x)               - Converts an integer to a Unicode character.
ord(x)                  - Converts a single character to its integer value.
hex(x)                  - Converts an integer to a hexadecimal string.
oct(x)                  - Converts an integer to an octal string.
'''