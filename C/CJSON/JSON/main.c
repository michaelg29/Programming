#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "json.h"

extern int errno;

int main()
{
    printf("Hello, world!\n");

    FILE *fp;
    fp = fopen("test.json", "rb");
    json OUT = json_readFile(fp);
    fclose(fp);

    //json OUT = json_type(JSON_OBJ);

    // json list = json_type(JSON_LIST);
    // json_list_add_string(&list, "str1");
    // json_list_add_float(&list, 1.29f);
    // json_list_add_null(&list);
    // json_list_add_int(&list, 332);
    // json_list_add_bool(&list, false);
    // json_list_add_string(&list, "hello");
    // json_obj_put(&OUT, "list1", &list);

    // json list2 = json_type(JSON_LIST);
    // json_list_add_null(&list2);
    // json_obj_put(&OUT, "list2", &list2);

    // json list3 = json_type(JSON_LIST);
    // json_obj_put(&OUT, "list3", &list3);

    // json_obj_put_string(&OUT, "str123", "asdfasdgf");
    // json_obj_put_float(&OUT, "float", 15325.3f);

    printf("start dump\n");
    int n;
    char *str = json_dump(OUT, true, &n);
    printf("%s\n", str);
    free(str);
    printf("end dump\n");

    // printf("start write\n");
    // FILE *fp;
    // fp = fopen("test.json", "wb");
    // json_writeFile(OUT, fp, true);
    // fclose(fp);
    // printf("end write\n");

    //int errnum = errno;
    //printf("Value of errno: %d\n", errno);
    //perror("Error printed by perror");
    //fprintf(stderr, "Error opening file: %s\n", strerror(errnum));

    printf("start free\n");
    json_freeDeep(&OUT);
    printf("end free\n");

    return 0;
}