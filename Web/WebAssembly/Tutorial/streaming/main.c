#include <stdio.h>
#include <emscripten.h>

int main() { 
   return 0;
}

EMSCRIPTEN_KEEPALIVE
int add(int a, int b) {
    return a + b;
}