#======================================assert==================================
def KelvinToFahrenheit(Temperature):
   assert (Temperature >= 0),"Colder than absolute zero!"
   return ((Temperature-273)*1.8)+32
#print(KelvinToFahrenheit(273))
#print(int(KelvinToFahrenheit(505.78)))
#print(KelvinToFahrenheit(-5))
'''
Outputs:

32.0
451
Traceback (most recent call last):
  File "exceptions.py", line 7, in <module>
    print(KelvinToFahrenheit(-5))
  File "exceptions.py", line 3, in KelvinToFahrenheit
    assert (Temperature >= 0),"Colder than absolute zero!"
AssertionError: Colder than absolute zero!

'''

#=====================================handling================================
def FahrenheitToKelvin(f):
    ret = (f - 32) * (5/9) + 273.15
    if ret < 0:
        raise("Invalid input, less than absolute zero", f)
    return ret

var = 0
x = -3000
try:
    var = KelvinToFahrenheit(x)
    var = x / 0

except AssertionError as Argument: # [error1, error2, etc]
    print("Assertion exception:", Argument)
except: # any exception
    print("Other exception")
else: # no exception
    print(var)
finally: # executes with or without exception
    print("Done")

#======================================user-defined===============================
class Networkerror(RuntimeError):
    def __init__(self, arg):
        self.args = arg

try:
   raise Networkerror("Bad hostname")
except Networkerror as e:
   print(e)