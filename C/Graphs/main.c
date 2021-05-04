#include "graph.h"
#include "strstream.h"
#include "minheap.h"
#include <stdlib.h>

typedef struct
{
    int v;
    int idx;
} test;

char *printVoid(void **arr, int n)
{
    strstream ret = strstream_alloc(n);
    strstream_concat(&ret, "{ ");
    for (int i = 1; i <= n; i++)
    {
        strstream_concat(&ret, "%d%s", ((test *)arr[i])->v, i == n ? " " : ", ");
    }

    strstream_concat(&ret, "}");

    return ret.str;
}

char *printArr(int *arr, int n)
{
    strstream ret = strstream_alloc(n);
    strstream_concat(&ret, "{ ");
    for (int i = 0; i < n; i++)
    {
        strstream_concat(&ret, "%d%s", arr[i], i == n - 1 ? " " : ", ");
    }

    strstream_concat(&ret, "}");

    return ret.str;
}

int intcmp(void *e1, void *e2)
{
    int i1 = ((test *)e1)->v;
    int i2 = ((test *)e2)->v;

    if (i1 > i2)
    {
        return 1;
    }
    else if (i1 == i2)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int intidx(void *e)
{
    return ((test *)e)->idx;
}

int main()
{
    printf("Hello, world!\n");

    int N = 5;

    int src[1] = {0};
    graph g = graph_new(ADJ_LIST, N, src, 1);
    graph_addDirectedWeightedEdge(&g, 0, 1, 10);
    graph_addDirectedWeightedEdge(&g, 0, 3, 5);
    graph_addDirectedWeightedEdge(&g, 1, 2, 2);
    graph_addDirectedWeightedEdge(&g, 1, 3, 2);
    graph_addDirectedWeightedEdge(&g, 2, 4, 2);
    graph_addDirectedWeightedEdge(&g, 3, 1, 1);
    graph_addDirectedWeightedEdge(&g, 3, 2, 4);
    graph_addDirectedWeightedEdge(&g, 3, 4, 6);
    graph_addDirectedWeightedEdge(&g, 4, 0, 3);
    graph_addDirectedWeightedEdge(&g, 4, 2, 7);

    printf("=======\n");

    int *d = malloc(N * sizeof(int));
    int *f = malloc(N * sizeof(int));
    int *p = malloc(N * sizeof(int));

    graph_dfsStart(&g, d, f, p);

    char *str = printArr(d, N);
    printf("%s\n", str);
    free(str);

    str = printArr(f, N);
    printf("%s\n", str);
    free(str);

    str = printArr(p, N);
    printf("%s\n", str);
    free(str);

    p = graph_dijkstra(&g, 0);
    str = printArr(p, N);
    printf("%s\n", str);
    free(str);
    free(p);

    graph_free(&g);

    return 0;
}