#==========================if, elif, else======================
x = 100
if x > 0:
    print("x is positive")
    if x % 2 == 0:
        print("x is even")
    else:
        print("x is odd")
elif x == 0:
    print("x = 0")
else:
    print ("x is negative")

#==========================loops========================================
#range
range(5)        # 0, 1, 2, 3, 4
range(3, 6)     # 3, 4, 5
range(3, 8, 2)  # 3, 5, 7           --- step of 2

#for loops
for x in range(5):
    print(x)

#while loops
count = 0
while count < 5:
    print(count)
    count += 1      # not ++

#break
count = 0
while True:
    print(count)
    count += 1
    if count >= 5:
        break       # stops loop

#continue
for x in range(10):
    if x % 2 == 0:
        continue    # go to next iteration without finishing this one
    print(x)

#pass
for letter in 'Python': 
   if letter == 'h':
      pass                              # does nothing, placeholder
      print 'This is pass block'
   print 'Current Letter :', letter