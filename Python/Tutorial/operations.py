#===========================arithmetic=========================
a = b = 1
c = a + b   # addition
c = a - b   # subtraction
c = a * b   # multiplication
c = a / b   # division
c = a**b    # exponent
c = a % b   # modulus
c = a // b  # floor division

#============================comparison========================
'''
==          equal
!=          not equal
<>          not equal
>           greater than
<           less than
>=          greater than or equal to
<=          less than or equal to
'''

#===========================assignment======================
c = a + b       # c = a + b
c += a          # c = c + a
c -= a          # c = c - a
c *= a          # c = c * a
c /= a          # c = c / a
c %= a          # c = c % a
c **= a         # c = c**a
c //= a         # c = c // a

#==========================bitwise========================
a = 60            # 60 = 0011 1100 
b = 13            # 13 = 0000 1101 
c = 0

# binary AND - if both are 1
c = a & b;        # 12 = 0000 1100

# binary OR - if either is 1
c = a | b;        # 61 = 0011 1101 

# binary XOR - if only one is 1
c = a ^ b;        # 49 = 0011 0001

# binary complement - flips bits
c = ~a;           # -61 = 1100 0011

# binary left shift - moves bits left (00110000 --> 11110000)
c = a << 2;       # 240 = 1111 0000

# binary right shift - moves bits right
c = a >> 2;       # 15 = 0000 1111

#============================logical operators================
#and, or, not
a = True
b = False
a and b         # false
a or b          # true
not(a and b)    # true

#============================membership========================
#used for seuqnces (strings, lists, tuples)
list = [0, 1, 2, 3, 4]
0 in list       # true
0 not in list   # false

#============================identity=======================
print(c is int)
print(c is not float)