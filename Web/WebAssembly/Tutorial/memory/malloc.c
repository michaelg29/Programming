#include <stdlib.h>
#include <emscripten.h>

int main() { return 0; }

EMSCRIPTEN_KEEPALIVE
int accumulate(int *arr, int n) {
    int sum = 0;
    while (n) {
        sum += arr[--n];
    }
    return sum;
}

EMSCRIPTEN_KEEPALIVE
const char* getString() {
    return "Hello World!";
}

EMSCRIPTEN_KEEPALIVE
void* wasmmalloc(int n) {
    return malloc((size_t)n);
    return 0;
}

EMSCRIPTEN_KEEPALIVE
void wasmfree(void *ptr) {
    free(ptr);
}