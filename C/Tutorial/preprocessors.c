#include <stdio.h>

/*
    main list
    #define - substitutes a preprocessor macro
    #include - inserts header file
    #undef - undefines preprocessor macro
    #ifdef - if macro defined
    #ifndef - if macro not defined
    #if - compile time condition
    #else
    #elif
    #endif - ends preprocessor conditional
    #error - prints error message on stderr
    #pragma - issue special commands to compiler
*/

#define MAXLENGTH 20
#undef MAXLENGTH
#define MAXLENGTH 21

#ifndef MESSAGE
#define MESSAGE "msg"
#endif

int main()
{
    // predefined macros
    printf("File :%s\n", __FILE__);
    printf("Date :%s\n", __DATE__);
    printf("Time :%s\n", __TIME__);
    printf("Line :%d\n", __LINE__);

    /*
        =======================
        preprocessor operators
    */

// parameterized macros
#define square(x) (x * x)
    printf("%f\n", square(5.0f));
#define MAX(x, y) x > y ? x : y
    printf("%f\n", MAX(5.0f, 9.00f));

// macro continuation
#define message_for(a, b) \
    printf(#a " and " #b "\n")

    message_for(Carole, Debra);

// token pasting - combines two arguments
#define tokenpaster(n) printf("token" #n " = %d", token##n)
    int token34 = 40;
    tokenpaster(34);

    return 0;
}