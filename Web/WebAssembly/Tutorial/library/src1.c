#include <emscripten.h>
#include "src2.h"

int main() { return 0; }

EMSCRIPTEN_KEEPALIVE
int sumOfFirstNInts(int n) {
    return divide(multiply(n, add(n, 1)), 2);
}