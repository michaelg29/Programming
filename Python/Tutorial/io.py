#=================================================screen==============================
print("Text output to screen")

#str = input("Enter input: ")
#print("Result:", str)

#================================================files=======================================
fo = open("test.txt", mode="w+")

'''
mode
r           read-only
rb          read-only in binary format
r+          read and write
rb+         read and write in binary
w           write-only
wb          write-only in binary
w+          write and read
wb+         write and read in binary
a           appending
ab          appending in binary format
a+          appending and reading
ab+         appending and reading in binary
'''

print("Name of the file: ", fo.name)
print("Closed or not : ", fo.closed)
print("Opening mode : ", fo.mode)

#writing
fo = open("test.txt", mode="w")
fo.write("Hello world!\nYeah, that's right.") # erases existing content

#reading
fo = open("test.txt", mode="r")
print(fo.tell)      # current position
fo.seek(0, 0)       # reposition cursor to ln, col
str = fo.read(10)   # reads 10 bytes
print(str)

fo.close()

#rename
import os
print(os.getcwd())                      # current directory
os.rename("test.txt", "test1.txt")      # rename file
os.remove("test1.txt")                  # remove file
os.rmdir("test-dir")                    # remove directory
os.mkdir("test-dir")                    # make directory
os.chdir("test-dir")                    # change directory