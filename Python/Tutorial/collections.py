#===================================================lists=============================================
#array of characters
str = "Hello world"

print(str)          # Prints complete string
print(str[0])       # Prints first character of the string
print(str[2:5])     # Prints characters starting from 3rd to 5th
print(str[2:])      # Prints string starting from 3rd character
print(str[:2])      # prints from beginning to 2nd character (non inclusive)
print(str * 2)      # Prints string two times
print(str + "TEST") # Prints concatenated string

#array of items
list = [ 'abcd', 786 , 2.23, 'john', 70.2 ]
tinylist = [123, 'john']

print(list)             # Prints complete list
print(list[0])          # Prints first element of the list
print(list[1:3])        # Prints elements starting from 2nd till 3rd 
print(list[2:])         # Prints elements starting from 3rd element
print(list[:2])         # start to second element
print(list[-2])         # prints second character from the right
print(tinylist * 2)     # Prints list two times
print(list + tinylist)  # Prints concatenated lists

list[2] = 555
del list[2]             # removes, list[2], list[3] moves left one, etc.

cmp(list, tinylist)
len(list)
max(list)
min(list)

list.append(52)         # appends 52 to list
list.count(52)          # count times 52 appears
list.extend(tinylist)   # appends tinylist to list
list.index(52)          # first index of object
list.insert(3, 1234)    # insert object at index
list.pop()              # removes and returns last object from list
list.remove(52)         # removes 52 from list
list.reverse()          # reverse objects
list.sort()             # sort

#tuples (cannot be changed)
tuple = ('abcd', 786 , 2.23, 'john', 70.2)
#same operations as list
#tuple[0] = 'a' ---- throws error
list(tuple)             # converts tuple to list

#================================================dictionaries======================================
dict = {}
dict['one'] = "This is one"
dict[2]     = "This is two"

tinydict = {'name': 'john','code':6734, 'dept': 'sales'}

print(dict['one'])       # Prints value for 'one' key
print(dict[2])           # Prints value for 2 key
print(tinydict)          # Prints complete dictionary
print(tinydict.keys())   # Prints all the keys
print(tinydict.values()) # Prints all the values

del dict[2]                 # removes key and value where key = 2
dict.clear()                # clears all entries
dict.copy()                 # returns copy
dict.fromkeys()             # returns new dictionary with keys from seq and values set to value
dict.get(key)               # gets value with key
dict.items()                # pairs
dict.keys()                 # keys
dict.values()               # values
dict.update(dict)           # appends dict to dict