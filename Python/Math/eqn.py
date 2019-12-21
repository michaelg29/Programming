import math

class function:
    TYPE_FUNCTION = "f"
    TYPE_OPERATOR = "o"

    def __init__(self, eval, t=TYPE_FUNCTION, prec=5, left=True):
        self.eval = eval
        self.t = t
        self.prec = prec
        self.left = left

class node:
    def __init__(self, val, func=True, unary=True):
        self.val = val
        self.func = func
        self.unary = unary

# constants dictionary
constants = {
    "pi": math.pi,
    "e": math.e,
    "tau": math.tau
}
constant_names = list(constants.keys())

unary_functions = {
    "sin": function(lambda x : math.sin(x)),
    "cos": function(lambda x : math.cos(x)),
    "tan": function(lambda x : math.tan(x)),
    "arcsin": function(lambda x : math.asin(x)),
    "arccos": function(lambda x : math.acos(x)),
    "arctan": function(lambda x : math.atan(x)),
    "sec": function(lambda x : 1 / math.cos(x)),
    "csc": function(lambda x : 1 / math.sin(x)),
    "cot": function(lambda x : 1 / math.tan(x)),
    "arcsec": function(lambda x : math.acos(1 / x)),
    "arccsc": function(lambda x : math.asin(1 / x)),
    "arccot": function(lambda x : math.atan(1 / x)),
    "ln": function(lambda x : math.log(x)),
    "sqrt": function(lambda x : math.sqrt(x)),
    "ceil": function(lambda x : math.ceil(x))
}

binary_functions = {
    "+": function(lambda x, y : x + y, function.TYPE_OPERATOR, 2),
    "-": function(lambda x, y : x - y, function.TYPE_OPERATOR, 2),
    "*": function(lambda x, y : x * y, function.TYPE_OPERATOR, 3),
    "/": function(lambda x, y : x / y, function.TYPE_OPERATOR, 3),
    "%": function(lambda x, y : x % y, function.TYPE_OPERATOR, 3),
    "^": function(lambda x, y : x ** y, function.TYPE_OPERATOR, 4, False),
    "max": function(lambda x, y : x if x >= y else y),
    "min": function(lambda x, y : x if x <= y else y),
    "log": function(lambda base, x : math.log(x, base))
}

functions = list(unary_functions.keys()) + list(binary_functions.keys())
operators = "+-*/%^"
left_brackets = "({["
right_brackets = "]})"

def isNumber(c):
    if type(c) is float:
        return True

    return c.isnumeric() or c in constants or c == '.'

def getNumVal(c):
    if type(c) in float:
        return c
    elif c in constants:
        return constants[c]
    else:
        return float(c)

def isFunction(c):
    return c in functions

def findElement(i, eqn, list):
    for el in list:
        n = len(el)
        if eqn[i:i+n] == el:
            return True, el, n

    return False, '', 1

def getPrecedence(op):
    if op in binary_functions:
        return binary_functions[op].prec

    return 0

def isLeftAssociative(op):
    return binary_functions[op].left

def RPN(eqn):
    TYPE_OP = "o"
    TYPE_CONST = "c"
    TYPE_FUNC = "f"
    TYPE_ELSE = "e"
    TYPE_LPAREN = "("
    TYPE_RPAREN = ")"

    queue = [] # output queue
    stack = [] # operator stack

    obj = ""
    type = ""

    eq_len = len(eqn)
    i = 0
    while i < eq_len:
        t = eqn[i]

        if t == ' ' or t == ',':
            # skip
            i += 1
            continue

        # parse value
        if isNumber(t):
            type = TYPE_CONST
            # find numerical value
            obj = t
            if i < len(eqn) - 1: # ensure has more
                while eqn[i + 1].isnumeric():
                    # add to string val if numerical
                    obj += eqn[i + 1]
                    i += 1
                    if i >= len(eqn) - 1:
                        break
            i += 1
            obj = float(obj)
        else:
            # determine if operator or function
            # find operator
            found, obj, n = findElement(i, eqn, functions)
            if found:
                if obj in operators:
                    type = TYPE_OP
                else:
                    type = TYPE_FUNC
            else:
                found, obj, n = findElement(i, eqn, constant_names)
                if found:
                    type = TYPE_CONST
                else:
                    if t in left_brackets:
                        type = TYPE_LPAREN
                    elif t in right_brackets:
                        type = TYPE_RPAREN
                    else:
                        type = TYPE_ELSE
            i += n


        #print(obj, queue, stack)

        if type == TYPE_CONST:
            # token is number
            queue.append(obj)
        elif type == TYPE_FUNC:
            # token is function
            stack.append(obj)
        elif type == TYPE_OP:
            # token is operator
            if len(stack) != 0:
                while (getPrecedence(stack[-1]) > getPrecedence(obj) \
                    or (getPrecedence(stack[-1]) == getPrecedence(obj) and isLeftAssociative(stack[-1]))) \
                    and stack[-1] not in left_brackets:
                    queue.append(stack.pop())
                    if len(stack) == 0:
                        break
            stack.append(obj)
        elif type == TYPE_LPAREN:
            # left bracket
            stack.append('(')
        elif type == TYPE_RPAREN:
            # right bracket
            while stack[-1] != '(':
                queue.append(stack.pop())
            stack.pop()
        else:
            print(f'not identified \'{t}\'')
            break

        #print(queue, stack)

    while len(stack) > 0:
        queue.append(stack.pop())

    return queue

def parse(rpn):
    stack = []

    for t in rpn:
        tr = None
        if isNumber(t):
            tr = node(t, False)
            tr.name = t
        else:
            if t in binary_functions.keys():
                # binary operator
                tr = node(binary_functions[t], True, False)
                tr.name = t

                a = stack.pop()
                b = stack.pop()

                if type(a) is node:
                    tr.right = a
                else:
                    tr.right = node(a, False)

                if type(b) is node:
                    tr.left = b
                else:
                    tr.left = node(b, False)
            elif t in unary_functions.keys():
                # unary operator
                tr = node(unary_functions[t])
                tr.name = t

                a = stack.pop()

                if type(a) is node:
                    tr.left = a
                else:
                    tr.left = node(a, False)
        stack.append(tr)

    return stack.pop()

def eval(tree, **vars):
    if tree.func:
        if tree.unary:
            return tree.val.eval(float(eval(tree.left)))
        else:
            return tree.val.eval(float(eval(tree.left)), float(eval(tree.right)))
    else:
        if tree.val in constants:
            return constants[tree.val]
        elif tree.val in vars:
            return vars[tree.val]
        return tree.val

if __name__ == "__main__":
    eqn = input("Equation: ")

    rpn = RPN(eqn)
    tree = parse(rpn)
    out = round(eval(tree), 8)

    print(f"Input:  {eqn}")
    print(f"RPN:    {rpn}")
    print(f"Result: {out}")
