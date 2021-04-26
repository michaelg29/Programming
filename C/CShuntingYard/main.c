#include <stdio.h>
#include <stdlib.h>
#include "sy_util.h"
#include "shuntingyard.h"
#include "lib/avl.h"
#include "lib/dynamicarray.h"

void printToken_(void *el)
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

int main(int argc, char **argv)
{
    printf("Hello, world!\n");

    SY_init();

    //sin(3^-2 * 27 / 4 * pi) + 1
    if (SY_registerVariable("e", 2.0))
    {
        printf("Registered into variable\n");
    }
    else
    {
        printf("Restricted variable\n");
    }

    if (SY_registerVariable("xda", 2.0))
    {
        printf("Registered into variable\n");
    }
    else
    {
        printf("Restricted variable\n");
    }

    dynamicarray RPNlist = RPN("-sin(2pi cos(2e)-sqrt(22))+-1+xda");
    dynarr_iterate(&RPNlist, printToken_);
    SY_tokenNode *tree = getEquationTree(RPNlist);

    printf("%.8f\n", evalTree(tree));

    SY_freeTokenList(&RPNlist);
    SY_freeNode(tree);

    SY_cleanup();

    printf("done\n");

    return 0;
}