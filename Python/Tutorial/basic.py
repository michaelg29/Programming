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