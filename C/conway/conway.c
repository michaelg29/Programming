#include "conway.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mod(int n, int d) {
    while (n < 0) {
        n += d;
    }

    return n % d;
}

void conway_init(conway *c, char wrap, int x, int y) {
    if (!c) {
        return;
    }

    if (x < 0 || y < 0) {
        return;
    }

    c->wrap = wrap;
    c->x = x;
    c->y = y;
    c->board = malloc(x * y);
    memset(c->board, 0, x * y);
}

void conway_seed(conway *c, char *seed, char empty) {
    for (int i = 0; i < c->x * c->y; i++) {
        c->board[i] = seed[i] != empty ? 1 : 0;
    }
}

void conway_seedTable(conway *c, char **seed, char empty) {
    for (int x = 0; x < c->x; x++) {
        for (int y = 0; y < c->y; y++) {
            c->board[x * c->y + y] = seed[x][y] != empty ? 1 : 0;
        }
    }
}

void conway_simulate(conway *c) {
    char *tmp = malloc(c->x * c->y);

    int idx = 0;
    for (int x = 0; x < c->x; x++) {
        for (int y = 0; y < c->y; y++) {
            // count active neighbors
            int activeNeighbors = 0;
            for (int xi = -1; xi <= 1; xi++) {
                for (int yi = -1; yi <= 1; yi++) {
                    if ((xi || yi) && conway_cell(c, x + xi, y + yi)) {
                        activeNeighbors++;
                    }
                }
            }

            if (c->board[idx] && (activeNeighbors == 2 || activeNeighbors == 3)) {
                // alive cells with 2 or more than 3 neighbors stay alive
                tmp[idx] = 1;
            }
            else if (!c->board[idx] && activeNeighbors == 3) {
                // dead cells with 3 neighbors becomes live
                tmp[idx] = 1;
            }
            else {
                // all others remain dead
                tmp[idx] = 0;
            }

            idx++;
        }
    }

    memcpy(c->board, tmp, c->x * c->y);
    free(tmp);
}

void conway_simulateN(conway *c, int n) {
    while (n--) {
        conway_simulate(c);
    }
}

void conway_clear(conway *c) {
    if (c && c->board) {
        memset(c->board, 0, c->x * c->y);
    }
}

void conway_destroy(conway *c) {
    if (c && c->board) {
        c->x = 0;
        c->y = 0;
        free(c->board);
    }
}

int conway_cell(conway *c, int x, int y) {
    if (c->wrap) {
        x = mod(x, c->x);
        y = mod(y, c->y);
    }
    else {
        if (x < 0 || x >= c->x ||
            y < 0 || y >= c->y) {
                return 0;
        }
    }

    return c->board[x * c->y + y];
}

char **conway_print(conway *c) {
    char **ret = malloc(c->x * sizeof(char*));

    for (int x = 0; x < c->x; x++) {
        ret[x] = malloc(c->y * sizeof(char) + 1);
        for (int y = 0; y < c->y; y++) {
            ret[x][y] = c->board[x * c->y + y] ? '0' : ' ';
        }
        ret[x][c->y] = '\0';
    }

    return ret;
}