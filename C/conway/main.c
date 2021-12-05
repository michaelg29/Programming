#include "conway.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void printConway(conway *c) {
    char **str = conway_print(c);

    for (int x = 0; x < c->x; x++) {
        printf("%s\n", str[x]);
        free(str[x]);
    }

    free(str);

    printf("\n\n");
}

int main() {
    printf("Hello, world!\n");

    #define X 20
    #define Y 100

    conway c;
    conway_init(&c, 1, X, Y);

    char *seed[X] = {
        "                                                                                                    ",
        "                                                                                                    ",
        "                              1                                                                     ",
        "                            1 1                                                                     ",
        "                  11      11            11                                                          ",
        "                 1   1    11            11                                                          ",
        "      11        1     1   11                                                                        ",
        "      11        1   1 11    1 1                                                                     ",
        "                1     1       1                                                                     ",
        "                 1   1                                                                              ",
        "                  11                                                                                ",
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                                                    ",
        "                                                                                                    "
    };
    conway_seedTable(&c, seed, ' ');

    // char *seed = "1          111                                    ";
    // conway_seed(&c, seed, ' ');

    int nr = 800;
    printf("%d\n", nr);
    printConway(&c);

    while (nr--) {
        //printf("%d\n", nr);
        conway_simulate(&c);
        printConway(&c);
        Sleep(100);
    }

    conway_destroy(&c);

    return 0;
}