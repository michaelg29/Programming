#include <stdio.h>
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
int test1() {
    return 200;
}

EMSCRIPTEN_KEEPALIVE
int test2() {
    return 300;
}

EMSCRIPTEN_KEEPALIVE
int main() { 
   int result = test1() + test2(); 
   return result; 
}

EMSCRIPTEN_KEEPALIVE
int add(int a, int b) {
    return a + b;
}