#exec(open(r"C:\Users\Michael Grieco\Documents\Python\first.py").read())

print("Hello")
the_world_is_flat = True
if the_world_is_flat:
    print("Be careful not to fall off!")
2+2

#floor division (round) use //
7 // 6
#1

#for exponents use **
6 ** 2
#36
9 ** .5
#3.0

#use _ to represent last printed value: (only works on IDLE)
#tax = 12.5 / 100
#price = 100.50
#price * tax
#12.5625
#price + _
#113.0625
#round(_, 2)
#113.06

#use raw strings with 'r' preceeding (doesn't see escape characters)
print(r'C:\some\name') # no new line

##########################################STRINGS###################################
#3 times 'un', followed by 'ium'
print(3 * 'un' + 'ium')

word = "Python"
print(word)

#character
print(word[0])
#character from end
print(word[-1])         #last character
print(word[-2])         #second last character

#substring (can use -ve indicies)
print(word[0:2])        #characters at 0 (included) to 2 (excluded)
print(word[:2])         #beginning to character at 2 (excluded)
print(word[2:])         #character at 2 (included) to end of string

print(len(word))

#strings are immutable (i.e cannot do: word[0] = 'a')

###########################################LISTS####################################
squares = [1, 4, 9, 16, 25]
print(squares) #[1, 4, 9, 16, 25]

#can access members just like in the strings section

squares += [36, 49, 64, 81, 100]
squares.append(121)
print(squares)

#lists are mutable (can change specific members)
letters = ['a', 'b', 'c', 'd', 'e', 'f', 'g']
print(letters)
len(letters) #7

letters[2:5] = ['C', 'D', 'E'] #modifies specific range of values
print(letters)

letters[2:5] = []   #removes values
letters[:] = []     #clears list

letters.insert(0, 'a')  #inserts at index

#nesting lists
a = ['a', 'b', 'c']
n = [1, 2, 3]
x = [a, n]
print(x)

##############################################CONTROL FLOW##########################
x = 42
#x = int(input("Please enter an integer: "))
if (x < 0):
    print('Input is negative')
elif (x == 0):
    print('Zero')
elif (x == 1):
    print('One')
else:
    print('Multiple')

#while loops
a, b = 0, 1
while (a < 10):
    print(a, end=', ', flush=True) #end keyword prevents new line after print
    a, b = b, a + b
    
print()
    
#for loops
words = ['cat', 'window', 'defenestrate']
for w in words:
    print(w, len(w), flush=True)
    
for i in range(5): #0 --> 4
    print(i, flush=True)
    
for n in range(len(words)):
    print(n, words[n], end=', ', flush=True)
    
for j in range(2, 10):
    for x in range(2, j):
        if j % x == 0:
            print(j, 'equals', x, '*', j//x)
            break
    else:
        #loop failed
        print(j, 'is a prime number')
        
for num in range(2, 10):
    if num % 2 == 0:
        print('Found an even number', num)
        continue
    print('Found a number')
    
#pass
#print('Press Ctrl+C')
#while True:
    #pass # waits for keyboard interrupt
    
#range
range(5)        #5 values starting from 0
range(5, 10)    #values from 5 to 10 (excluded)
range(0, 10, 3) #values from 0 to 10 (excluded) in multiples of 3

########################################FUNCTIONS###################################
def fib(n): #write Fibonacci series up to n
    a, b = 0, 1
    while (a < n):
        print(a, end=' ', flush=True)
        a, b = b, a+b
    print()
    
def fib2(n): #return list of Fibonacci series up to n
    result = []
    a, b = 0, 1
    while (a < n):
        result.append(a)
        a, b = b, a+b
    return result

f = fib
f(100)

f2 = fib2
print(list(f2(100)))

def add(a, L=[]): #default shared in execution
	L.append(a)
	return L

print(add(1)) # [1]
print(add(2)) # [1, 2]
print(add(3)) # [1, 2, 3]

def add_(a, L=None): #L not shared between all function calls
	if L is None:
		L = []
	L.append(a)
	return L

print(add_(1)) # [1]
print(add_(2)) # [2]
print(add_(3)) # [3]

def dog(voltage, state='bark', action='voom'):
	print('The dog goes', state, 'and then', action)

dog(voltage=10000, action='runs', state='sniffs')
dog(action='barks', state='whimpering', voltage=1)
dog(10000, action='runs', state='sniffs')
#dog(1000, voltage=1) -- duplicate argument, throws error

def cheeseshop(kind, *arguments, **keywords): # * specifies a list, ** specifies a dictionary
	print('--Do you have any', kind + '?')
	print('--I\'m sorry, we\'re all out of', kind)
	for arg in arguments:
		print(arg)
	print('-' * 40)
	for kw in keywords: #returns list of keys
		print(kw, ':', keywords[kw])

cheeseshop("Limburger", "It's very runny, sir.",
           "It's really very, VERY runny, sir.",
           shopkeeper="Michael Palin",
           client="John Cleese",
           sketch="Cheese Shop Sketch")

#def ask_ok(prompt, retries=4, reminder='Please try again!'): #default args
#    while True:
#        ok = input(prompt)
#        if (ok in ('y', 'ye', 'yes')):
#            return True;
#        elif (ok in ('n', 'no', 'nope')):
#            return False;
#        
#        retries -= 1
#        if (retries < 0):
#            return False
#            
#        print(reminder, retries, 'tries left.')
        
#ask_ok('Continue?')

########################################SYSTEM COMMANDS#############################
# import only system from os
#from os import system, name
 
# import sleep to show output for some time period
#from time import sleep
    
# define our clear function
#def clear():
#	sleep(5)
#    # for windows
#    if name == 'nt':
#        _ = system('cls')
# 
#    # for mac and linux(here, os.name is 'posix')
#    else:
#        _ = system('clear')
 
# now call function we defined above
#clear()