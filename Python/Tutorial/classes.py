class Employee:
    'documentation string'
    empCount = 0
    __ssn = 213213123                       # hidden

    def __init__(self, name, salary):       # constructor
        self.name = name
        self.salary = salary
        Employee.empCount += 1

    def __del__(self):
        Employee.empCount -= 1

    def displayCount(self):
        print("Total employee count: %d" % Employee.empCount)

    def displayEmployee(self):
        print("Name: %15s; Salary: $%15.2f" % (self.name, self.salary))

#=======================================class attributes=====================================
Employee.__dict__
Employee.__doc__
Employee.__name__
Employee.__module__
Employee.__bases__

emp1 = Employee("Michael", 20000)
emp1.displayEmployee()
emp1.age = 25                   # add attribute
del emp1.age                    # delete attribute

if not(hasattr(emp1, 'age')):   # has attribute
    setattr(emp1, 'age', 26)    # set attribute

print(getattr(emp1, 'age'))     # get attribute
delattr(emp1, 'age')            # delete attribute

print("Employee count: %d" % Employee.empCount)
del emp1
print("Employee count: %d" % Employee.empCount)

#===========================================inheritance=================================
class Parent:        # define parent class
    parentAttr = 100
    __salary = 230123

    def __init__(self):
        print("Calling parent constructor")

    def parentMethod(self):
        print('Calling parent method')

    def setAttr(self, attr):
        Parent.parentAttr = attr

    def getAttr(self):
        print("Parent attribute :", Parent.parentAttr)

    def myMethod(self):
        print("calling parent method")

    def __str__(self):          # returns conversion to string
        pass

    def __add__(self, other):   # parent1 + parent2
        pass

class Child(Parent): # define child class
    def __init__(self):
        print("Calling child constructor")

    def childMethod(self):
        print('Calling child method')

    def myMethod(self):         # override
        print("calling child method")

c = Child()          # instance of child
c.childMethod()      # child calls its method
c.parentMethod()     # calls parent's method
c.setAttr(200)       # again call parent's method
c.getAttr()          # again call parent's method
c.myMethod()

print(issubclass(Child, Parent))
print(isinstance(c, Child))
print(isinstance(c, Parent))