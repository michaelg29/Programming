#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <conio.h>
#include <process.h>

typedef struct {
    int *a;
    int id;
} data;

unsigned int func(void *arg) {
    data *d = (data*)arg;
    printf("Thread %d started\n", d->id);

    for (int i = 0; i < 10; i++) {
        (*(d->a))++;
        printf("Thread %d incrementing to %d\n", d->id, *(d->a));
    }

    _endthread();

    return d->id;
}

int main() {
    int a = 0;

    data d1 = { &a, 1 };
    data d2 = { &a, 2 };

    printf("Beginning...\n");

    unsigned id1;
    unsigned threadid1 = _beginthreadex(0, 0, func, &d1, 0, &id1);
    unsigned id2;
    unsigned threadid2 = _beginthreadex(0, 0, func, &d2, 0, &id2);

    CloseHandle(&threadid1);
    CloseHandle(&threadid2);
}