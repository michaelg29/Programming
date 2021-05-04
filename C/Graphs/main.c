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

    int N = 6;

    int src[1] = {0};
    graph g = graph_new(ADJ_LIST, N, src, 1);
    graph_addDirectedWeightedEdge(&g, 0, 1, 16);
    graph_addDirectedWeightedEdge(&g, 0, 2, 13);
    graph_addDirectedWeightedEdge(&g, 1, 3, 12);
    graph_addDirectedWeightedEdge(&g, 2, 1, 4);
    graph_addDirectedWeightedEdge(&g, 2, 4, 14);
    graph_addDirectedWeightedEdge(&g, 3, 2, 9);
    graph_addDirectedWeightedEdge(&g, 3, 5, 20);
    graph_addDirectedWeightedEdge(&g, 4, 3, 7);
    graph_addDirectedWeightedEdge(&g, 4, 5, 4);

    char *str = graph_toString(&g);
    printf("%s\n", str);
    free(str);

    printf("=======\n");

    int maxFlow;
    graph flow = graph_fordFulkerson(&g, 0, 5, &maxFlow);
    str = graph_toString(&flow);
    printf("%d\n%s\n", maxFlow, str);
    free(str);

    graph_free(&g);
    graph_free(&flow);

    return 0;
}