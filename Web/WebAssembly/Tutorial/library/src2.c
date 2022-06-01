#include "src2.h"
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
int multiply(int a, int b) {
    return a * b;
}

EMSCRIPTEN_KEEPALIVE
int divide(int a, int b) {
    return a / b;
}

EMSCRIPTEN_KEEPALIVE
int add(int a, int b) {
    return a + b;
}

EMSCRIPTEN_KEEPALIVE
int subtract(int a, int b) {
    return a - b;
}