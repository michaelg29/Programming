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

// use token pasting to create template
#define new_scalar_type(name, type)        \
    typedef struct                         \
    {                                      \
        type value;                        \
    } name;                                \
    inline name name##_(type v)            \
    {                                      \
        name t;                            \
        t.value = v;                       \
        return t;                          \
    }                                      \
    inline name add_##name(name a, name b) \
    {                                      \
        name t;                            \
        t.value = a.value + b.value;       \
        return t;                          \
    }                                      \
    inline name sub_##name(name a, name b) \
    {                                      \
        name t;                            \
        t.value = a.value - b.value;       \
        return t;                          \
    }                                      \
    inline type val_##name(name a)         \
    {                                      \
        return a.value;                    \
    }

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

    /*
        ===========================
        Token pasting (##) operator

        concatenates non-whitespace characters around it
        creates new tokens from identifiers, keywords (int, etc), literals, operators
    */

// token pasting - combines two arguments
#define tokenpaster(n) printf("token" #n " = %d\n", token##n)
    int token34 = 40;
    tokenpaster(34); // gets token34 from memory

#define increment(x) +## + u
    int u = 2;
    printf("\n%d\n", increment(u));

    // creates new type called age with value int
    new_scalar_type(age, int);

    age a = age_(5);
    age b = age_(2);
    printf("%d\n", val_age(a));
    printf("%d\n", val_age(add_age(a, b)));

#define convert(from, to, conversion, from_type, to_type) \
    to_type convert_##from##_to_##to(from_type f)         \
    {                                                     \
        return conversion;                                \
    }

    convert(f, c, (f - 32) * 5.0 / 9.0, float, float);
    printf("%f F = %f C\n", 70.0f, convert_f_to_c(70.0f));

    return 0;
}