#include <stdio.h>
#include <stdlib.h>
#include "sy_util.h"
#include "shuntingyard.h"
#include "lib/avl.h"
#include "lib/dynamicarray.h"

int main(int argc, char **argv)
{
    printf("Hello, world!\n");

    sy_init();

    //sin(3^-2 * 27 / 4 * pi) + 1
    dynamicarray RPNlist = RPN("-sin(2pi cos(2e)-sqrt(22))+-1");
    SY_tokenNode *tree = getEquationTree(RPNlist);

    printf("%.8f\n", evalTree(tree));

    dynarr_free(&RPNlist);
    SY_freeNode(tree);

    sy_cleanup();

    return 0;
}