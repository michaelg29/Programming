var1 = "Hello world!"
var2 = "Python programming"

# same functions as list methods (collections.py)

#=======================================escape characters==================================
'''
Backslash notation  Hexadecimal character	Description
\a	                0x07	                Bell or alert
\b	                0x08	                Backspace
\cx	 	                                    Control-x
\C-x	 	                                Control-x
\e	                0x1b	                Escape
\f	                0x0c	                Formfeed
\M-\C-x	 	                                Meta-Control-x
\n	                0x0a	                Newline
\nnn	 	                                Octal notation, where n is in the range 0.7
\r	                0x0d	                Carriage return
\s	                0x20	                Space
\t	                0x09	                Tab
\v	                0x0b	                Vertical tab
\x	 	                                    Character x
\xnn	 	                                Hexadecimal notation, where n is in the range 0.9, a.f, or A.F
'''

#=========================================string operators============================
var1 + var2         # concatenation
var1 * 20           # repetition
var1[1]             # slice
var1[1:3]           # range slice
R'\n'               # raw string, outputs \n

#========================================formatting===============================
print("My name is %s and weight is %d kg!" % ('Zara', 21))
n = 555323.23223
print("%0+20.2f" % n) # +0000000000555323.23
'''
Format Symbol	Conversion
%c	            character
%s	            string conversion via str() prior to formatting
%i	            signed decimal integer
%d	            signed decimal integer
%u	            unsigned decimal integer
%o	            octal integer
%x	            hexadecimal integer (lowercase letters)
%X	            hexadecimal integer (UPPERcase letters)
%e	            exponential notation (with lowercase 'e')
%E	            exponential notation (with UPPERcase 'E')
%f	            floating point real number
%g	            the shorter of %f and %e
%G	            the shorter of %f and %E

Symbol (%<SYMBOL><TYPE>)    Functionality
m.n                         width or precision (length of m with n decimal places, drop '.b1' with non numbers)
-                           left justification
+                           display sign
(space character)           leave blank space before positive number
#                           add octal 0x or 0X (depends on what format symbol)
0                           pad from left with zeros instead of spaces
%                           gives single literal '%'
'''

#============================================triple quotes================================
para_str = """this is a long string that is made up of
several lines and non-printable characters such as
TAB ( \t ) and they will show up that way when displayed.
NEWLINEs within the string, whether explicitly given like
this within the brackets [ \n ], or just a NEWLINE within
the variable assignment will also show up.
"""

#===============================================string methods=============================
str = "string"
list = [ 'abcd', 786 , 2.23, 'john', 70.2 ]

str.capitalize()                                # capitalizes first letter
str.center(50, ' ')                             # centers in width of 50, pads using ' '
str.count('r', beg=0, end=len(str))             # number occurrences of 'r'
str.decode(encoding='UTF-8', errors='strict')
str.encode(encoding='UTF-8', errors='strict')
str.startswith('g')                             # starts with 'g'
str.endswith('g', beg=0, end=len(str))          # ends with 'g'
str.expandtabs(tabsize=8)                       # sets tab spacing
str.find('r', beg=0, end=len(str))              # index of 'r'
str.rfind('r', beg=0, end=len(str))             # index of 'r' starting in back
str.index('r', beg=0, end=len(str))             # index of 'r', throws exception if not found
str.rindex('r', beg=0, end=len(str))            # index of 'r', throws exception if not found, starting in back
str.isalnum()                                   # all alphanumeric characters
str.isalpha()                                   # all letters
str.isdigit()                                   # all digits
str.islower()                                   # all lowercase letters
str.isupper()                                   # all uppercase letters
str.isnumeric()                                 # all numbers
str.isdecimal()                                 # all decimal characters
str.isspace()                                   # all spaces
str.istitle()                                   # titlecased
str.title()                                     # titlecases string (all words begin with uppercase, everything else lowercase)
join(list)                                      # joins all items of list in string
len(str)                                        # length
str.ljust(15, ' ')                              # left justifies, pads with ' '
str.rjust(15, ' ')                              # right justifies, pads with ' '
str.lower()                                     # converts to lower
str.upper()                                     # converts to upper
max(str)                                        # max alphabetical character
min(str)                                        # min alphabetical character
str.replace('r', '3', 4)                        # replace 'r' with '3' a maximum of 4 times
str.lstring()                                   # removes whitespace on left side
str.rstrip()                                    # removes whitespace on right side
str.strip()                                     # str.lstrip() and str.rstrip()
str.split(' ', 5)                               # splits at ' ' at maximum of 5 times
str.swapcase()                                  # inverts case for all letters


#string translate
from string import maketrans

intab = "aeiou"
outtab = "12345"
trantab = maketrans(intab, outtab)

str = "this is string example....wow!!!"
print(str.translate(trantab))                    # th3s 3s str3ng 2x1mpl2....w4w!!!