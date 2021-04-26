#include "shuntingyard.h"

#include "sy_util.h"
#include "sy_functions.h"
#include "lib/strstream.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

avl *functions;
avl *constants;
SY_token *multiplicationToken;

void printNode(avl *node, bool recurse)
{
    printf("{ %s: %s; %d; L: %s; R: %s; }\n",
           (char *)node->key, ((SY_token *)node->val)->val.strVal,
           node->height,
           node->left ? (char *)node->left->key : "null",
           node->right ? (char *)node->right->key : "null");

    if (recurse)
    {
        if (node->left)
        {
            printNode(node->left, recurse);
        }
        if (node->right)
        {
            printNode(node->right, recurse);
        }
    }
}

void sy_init()
{
    functions = avl_createEmptyRoot(strkeycmp);
    functions = avl_insert(functions, "sin", SY_createDefaultUnaryFunction("sin", sin));
    functions = avl_insert(functions, "cos", SY_createDefaultUnaryFunction("cos", cos));
    functions = avl_insert(functions, "tan", SY_createDefaultUnaryFunction("tan", tan));
    functions = avl_insert(functions, "asin", SY_createDefaultUnaryFunction("asin", asin));
    functions = avl_insert(functions, "acos", SY_createDefaultUnaryFunction("acos", acos));
    functions = avl_insert(functions, "atan", SY_createDefaultUnaryFunction("atan", atan));
    functions = avl_insert(functions, "csc", SY_createDefaultUnaryFunction("csc", csc));
    functions = avl_insert(functions, "sec", SY_createDefaultUnaryFunction("sec", sec));
    functions = avl_insert(functions, "cot", SY_createDefaultUnaryFunction("cot", cot));
    functions = avl_insert(functions, "acsc", SY_createDefaultUnaryFunction("acsc", acsc));
    functions = avl_insert(functions, "asec", SY_createDefaultUnaryFunction("asec", asec));
    functions = avl_insert(functions, "acot", SY_createDefaultUnaryFunction("acot", acot));
    functions = avl_insert(functions, "abs", SY_createDefaultUnaryFunction("abs", absf));
    functions = avl_insert(functions, "ln", SY_createDefaultUnaryFunction("ln", log));
    functions = avl_insert(functions, "log2", SY_createDefaultUnaryFunction("log2", log2));
    functions = avl_insert(functions, "log10", SY_createDefaultUnaryFunction("log10", log10));
    functions = avl_insert(functions, "sqrt", SY_createDefaultUnaryFunction("sqrt", sqrt));
    functions = avl_insert(functions, "exp", SY_createDefaultUnaryFunction("exp", exp));

    functions = avl_insert(functions, "+", SY_createBinaryFunction("+", add, OPERATOR, 2, true));
    functions = avl_insert(functions, "-", SY_createBinaryFunction("-", subtract, OPERATOR, 2, true));

    multiplicationToken = SY_createBinaryFunction("*", multiply, OPERATOR, 3, true);
    functions = avl_insert(functions, "*", multiplicationToken);
    functions = avl_insert(functions, "/", SY_createBinaryFunction("/", divide, OPERATOR, 3, true));
    functions = avl_insert(functions, "^", SY_createBinaryFunction("^", pow, OPERATOR, 4, false));
    functions = avl_insert(functions, "min", SY_createDefaultBinaryFunction("min", minf));
    functions = avl_insert(functions, "max", SY_createDefaultBinaryFunction("max", maxf));

    constants = avl_createEmptyRoot(strkeycmp);
    constants = avl_insert(constants, "pi", SY_createTokenConstantString("pi", 4.0 * atan(1.0), true));
    constants = avl_insert(constants, "e", SY_createTokenConstantString("e", exp(1.0), true));
}

void sy_cleanup()
{
    avl_freeDeep(functions);
    avl_freeDeep(constants);
}

dynamicarray parseTokens(char *str)
{
    unsigned int n = strlen(str);
    strstream eqn = strstream_alloc(n);
    strstream_concat(&eqn, "%s", str);

    dynamicarray ret = dynarr_defaultAllocate();

    tokentype prevType = ELSE;
    tokentype curType = ELSE;
    int objLength = 0;
    for (unsigned int i = 0; i < n; i += objLength)
    {
        char c = eqn.str[i];

        SY_token *obj = NULL;
        objLength = 1;

        if (c == ' ' || c == ',')
        {
            continue;
        }
        else if (c == '(')
        {
            curType = LPAREN;
            obj = SY_createTokenLParen();
        }
        else if (c == ')')
        {
            curType = RPAREN;
            obj = SY_createTokenRParen();
        }
        else
        {
            if (c == '-')
            {
                if (prevType == CONSTANT || prevType == RPAREN)
                {
                    // subtraction sign
                    curType = OPERATOR;
                    obj = SY_createTokenOperator("-");
                    objLength = 1;
                }
            }
            if (SY_charIsNumber(c) && curType != OPERATOR)
            {
                bool acceptDecimal = true;
                curType = CONSTANT;

                if (c == '.')
                {
                    // cannot have another decimal or a negative sign
                    acceptDecimal = false;
                }

                int nextI = i + 1;
                while (nextI < n && SY_charIsNumber(eqn.str[nextI]))
                {
                    if (eqn.str[nextI] == '-')
                    {
                        break;
                    }
                    if (eqn.str[nextI] == '.')
                    {
                        if (!acceptDecimal)
                        {
                            break;
                        }
                        acceptDecimal = false;
                    }
                    nextI++;
                }
                objLength = nextI - i;

                if (objLength == 1 && c == '-')
                {
                    // implicity insert a -1
                    obj = SY_createTokenConstant(-1.0);
                }
                else
                {
                    obj = SY_createTokenConstant(atof(strstream_substrLength(&eqn, i, objLength)));
                }
            }
            else
            {
                obj = SY_findElement(i, &eqn, functions, &objLength);
                if (objLength)
                {
                    // found function
                    curType = obj->type;
                }
                else
                {
                    obj = SY_findElement(i, &eqn, constants, &objLength);
                    curType = CONSTANT;
                    if (!objLength)
                    {
                        obj = SY_createTokenConstant(0.0);
                    }
                }

                if ((curType == FUNCTION || curType == CONSTANT) && !(prevType == ELSE || prevType == OPERATOR || prevType == LPAREN))
                {
                    // implicitly add multiplication sign
                    dynarr_addLast(&ret, multiplicationToken);
                }
            }
        }

        prevType = curType;
        curType = ELSE;

        dynarr_addLast(&ret, obj);
    }

    return ret;
}

void printToken(void *el)
{
    SY_token *t = el;
    switch (t->type)
    {
    case CONSTANT:
        printf("%f ", t->val.constVal);
        break;
    case FUNCTION:
    case OPERATOR:
        printf("%s ", t->val.funcVal.name);
        break;
    case ELSE:
        printf("nil ");
        break;
    default:
        printf("%s ", t->val.strVal);
        break;
    }
}

dynamicarray RPN(char *str)
{
    dynamicarray tokens = parseTokens(str);
    dynarr_iterate(&tokens, printToken);
    printf("\n");

    unsigned int n = tokens.size;
    dynamicarray ret = dynarr_allocate(n >> 1);
    dynamicarray stack = dynarr_defaultAllocate();

    for (unsigned int i = 0; i < n; i++)
    {
        SY_token *t = tokens.list[i];
        tokentype type = t->type;
        SY_token *lastStack = stack.size ? stack.list[stack.size - 1] : NULL;

        switch (type)
        {
        case CONSTANT:
        case CONSTANTSTR:
            dynarr_addLast(&ret, t);
            break;
        case FUNCTION:
            dynarr_addLast(&stack, t);
            break;
        case OPERATOR:
            if (stack.size)
            {
                /*
                    stk = stack top = last_stack
                    obj = obj

                        stk is not a left bracket
                    AND
                                stk is a function
                            AND
                                stk is not an operator
                        OR
                            stk has a higher precedence than obj
                        OR
                                they have equal precedence
                            AND
                                stk is left associative
                */
                while (
                    lastStack->type != LPAREN &&
                    (SY_isFunction(lastStack) ||
                     SY_getPrecedence(lastStack) > SY_getPrecedence(t) ||
                     (SY_getPrecedence(lastStack) == SY_getPrecedence(t) &&
                      SY_isLeftAssociative(lastStack))))
                {
                    // pop from the stack to the queue
                    dynarr_addLast(&ret, dynarr_removeLast(&stack));
                    if (!stack.size)
                    {
                        break;
                    }
                    lastStack = stack.list[stack.size - 1];
                }
            }
            dynarr_addLast(&stack, t);
            break;
        case LPAREN:
            dynarr_addLast(&stack, t);
            break;
        case RPAREN:
            while (lastStack && lastStack->type != LPAREN)
            {
                // pop from the stack to the queue
                dynarr_addLast(&ret, dynarr_removeLast(&stack));
                if (!stack.size)
                {
                    break;
                }
                lastStack = stack.list[stack.size - 1];
            }
            SY_freeToken(dynarr_removeLast(&stack));
            break;
        default:
            dynarr_free(&stack);
            dynarr_free(&tokens);
            return ret;
        };
    }

    while (stack.size)
    {
        // pop from the stack to the queue
        dynarr_addLast(&ret, dynarr_removeLast(&stack));
    }

    dynarr_free(&stack);

    return ret;
}

char *RPNstring(char *str)
{
    dynamicarray RPNtokens = RPN(str);
    unsigned int n = RPNtokens.size;

    strstream ret = strstream_alloc(n);

    for (unsigned int i = 0; i < n; i++)
    {
        SY_token *t = RPNtokens.list[i];
        switch (t->type)
        {
        case CONSTANT:
            strstream_concat(&ret, "%f ", t->val.constVal);
            break;
        case CONSTANTSTR:
            strstream_concat(&ret, "%s ", t->val.namedConstVal.name);
        case FUNCTION:
        case OPERATOR:
            strstream_concat(&ret, "%s ", t->val.funcVal.name);
            break;
        default:
            strstream_concat(&ret, "%s ", t->val.strVal);
        }
    }

    if (ret.size)
    {
        // retreat past last space
        strstream_retreat(&ret, 1);
    }

    dynarr_free(&RPNtokens);

    return ret.str;
}

SY_tokenNode *getEquationTree(dynamicarray RPN)
{
    dynamicarray stack = dynarr_defaultAllocate();

    dynarr_iterator it = dynarr_iterator_new(&RPN);
    SY_token *cur = NULL;

    while ((cur = dynarr_iterator_next(&it)))
    {
        if (cur->type == CONSTANT || cur->type == CONSTANTSTR)
        {
            // push number node
            dynarr_addLast(&stack, SY_createNode(cur));
        }
        else
        {
            // function
            SY_tokenNode *func = SY_createNode(cur);
            if (SY_isUnary(cur))
            {
                // unary function
                if (stack.size)
                {
                    func->left = dynarr_removeLast(&stack);
                }
            }
            else
            {
                // set right child (2nd argument)
                if (stack.size)
                {
                    func->right = dynarr_removeLast(&stack);
                }

                // set left child (1st argument)
                if (stack.size)
                {
                    func->left = dynarr_removeLast(&stack);
                }
            }
            dynarr_addLast(&stack, func);
        }
    }

    SY_tokenNode *ret = stack.size ? stack.list[stack.size - 1] : NULL;
    dynarr_free(&stack);

    return ret;
}

double evalTree(SY_tokenNode *tree)
{
    if (tree->t->type == OPERATOR || tree->t->type == FUNCTION)
    {
        if (SY_isUnary(tree->t))
        {
            if (tree->left)
            {
                return SY_eval(tree->t, evalTree(tree->left), 0.0);
            }
        }
        else
        {
            if (tree->left && tree->right)
            {
                return SY_eval(tree->t,
                               evalTree(tree->left),
                               evalTree(tree->right));
            }
        }
    }
    else
    {
        return SY_eval(tree->t, 0.0, 0.0);
    }

    return 0.0;
}

double SY_eval(SY_token *t, double x, double y)
{
    if (t->type == FUNCTION || t->type == OPERATOR)
    {
        return SY_isUnary(t) ? SY_evalUnary(t, x) : SY_evalBinary(t, x, y);
    }
    else if (t->type == CONSTANT)
    {
        return t->val.constVal;
    }
    else if (t->type == CONSTANTSTR)
    {
        return t->val.namedConstVal.value;
    }

    return 0.0;
}

bool sy_registerVariable(char *name, double value)
{
    SY_token *t = avl_get(constants, name);
    if (t)
    {
        if (t->val.namedConstVal.restricted)
        {
            return false;
        }

        t->val.namedConstVal.value = value;
    }
    else
    {
        t = SY_createTokenConstantString(name, value, false);
        constants = avl_insert(constants, name, t);
    }

    return true;
}