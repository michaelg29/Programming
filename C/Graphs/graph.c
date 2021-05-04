#include "graph.h"
#include "minheap.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

edge *createEdge(int v1, int v2)
{
    edge *ret = malloc(sizeof(edge));

    ret->v1 = v1;
    ret->v2 = v2;
    ret->weight = 0;

    return ret;
}

edge *createWeightedEdge(int v1, int v2, int weight)
{
    edge *ret = malloc(sizeof(edge));

    ret->v1 = v1;
    ret->v2 = v2;
    ret->weight = weight;

    return ret;
}

graph graph_new(char mode, int n, int *sources, int noSources)
{
    graph ret = {NULL, mode, n, 0, sources, noSources};

    if (mode)
    {
        ret.adjacencyLists = malloc(n * sizeof(dynamicarray));
        for (int i = 0; i < n; i++)
        {
            ret.adjacencyLists[i] = dynarr_defaultAllocate();
        }
    }
    else
    {
        ret.adjacencyMatrix = malloc(n * sizeof(int *));
        for (int i = 0; i < n; i++)
        {
            ret.adjacencyMatrix[i] = malloc(n * sizeof(int));
            for (int j = 0; j < n; j++)
            {
                ret.adjacencyMatrix[i][j] = 0;
            }
        }
    }

    return ret;
}

void graph_addVertex(graph *g)
{
    int n = g->n + 1;

    if (g->adjacencyMode)
    {
        dynamicarray *oldMem = g->adjacencyLists;
        g->adjacencyLists = realloc(g->adjacencyLists, n * sizeof(dynamicarray));
        if (!g->adjacencyLists)
        {
            g->adjacencyLists = malloc(n * sizeof(dynamicarray));
            memcpy(g->adjacencyLists, oldMem, g->n * sizeof(dynamicarray));

            free(oldMem);
        }
    }
    else
    {
        int **newAdjacencyMatrix = malloc(n * sizeof(int *));
        for (int i = 0; i < n; i++)
        {
            newAdjacencyMatrix[i] = malloc(n * sizeof(int));
            if (i < n - 1)
            {
                int j = 0;
                for (j = 0; j < n - 1; j++)
                {
                    newAdjacencyMatrix[i][j] = g->adjacencyMatrix[i][j];
                }
                newAdjacencyMatrix[i][j] = 0;
            }
            else
            {
                for (int j = 0; j < n; j++)
                {
                    newAdjacencyMatrix[i][j] = 0;
                }
            }
        }

        for (int i = 0; i < g->n; i++)
        {
            free(g->adjacencyMatrix[i]);
        }
        free(g->adjacencyMatrix);

        g->adjacencyMatrix = newAdjacencyMatrix;
    }

    g->n = n;
}

void graph_addDirectedEdge(graph *g, int v1, int v2)
{
    g->noEdges++;
    if (g->adjacencyMode)
    {
        dynarr_addLast(g->adjacencyLists + v1, createEdge(v1, v2));
    }
    else
    {
        g->adjacencyMatrix[v1][v2] = !0;
    }
}

void graph_addDirectedWeightedEdge(graph *g, int v1, int v2, int weight)
{
    g->noEdges++;
    if (g->adjacencyMode)
    {
        dynarr_addLast(g->adjacencyLists + v1, createWeightedEdge(v1, v2, weight));
    }
    else
    {
        g->adjacencyMatrix[v1][v2] = weight;
    }
}

void graph_addUndirectedEdge(graph *g, int v1, int v2)
{
    g->noEdges++;
    if (g->adjacencyMode)
    {
        dynarr_addLast(g->adjacencyLists + v1, createEdge(v1, v2));
        dynarr_addLast(g->adjacencyLists + v2, createEdge(v2, v1));
    }
    else
    {
        g->adjacencyMatrix[v1][v2] = !0;
        g->adjacencyMatrix[v2][v1] = !0;
    }
}

void graph_addUndirectedWeightedEdge(graph *g, int v1, int v2, int weight)
{
    g->noEdges++;
    if (g->adjacencyMode)
    {
        dynarr_addLast(g->adjacencyLists + v1, createWeightedEdge(v1, v2, weight));
        dynarr_addLast(g->adjacencyLists + v2, createWeightedEdge(v2, v1, weight));
    }
    else
    {
        g->adjacencyMatrix[v1][v2] = weight;
        g->adjacencyMatrix[v2][v1] = weight;
    }
}

void graph_free(graph *g)
{
    if (g)
    {
        if (g->adjacencyMode)
        {
            for (int i = 0; i < g->n; i++)
            {
                dynarr_freeDeep(g->adjacencyLists + i);
            }
            free(g->adjacencyLists);
        }
        else
        {
            for (int i = 0; i < g->n; i++)
            {
                free(g->adjacencyMatrix + i);
            }
            free(g->adjacencyMatrix);
        }
    }
}

graph graph_copy(graph *g)
{
    graph ret = {NULL, g->adjacencyMode, g->n, g->noEdges, g->sources, g->noSources};

    if (g->adjacencyMode)
    {
        ret.adjacencyLists = malloc(g->n * sizeof(dynamicarray));
        for (int i = 0; i < ret.n; i++)
        {
            ret.adjacencyLists[i] = dynarr_allocate(g->adjacencyLists[i].size);
            for (int j = 0; j < g->adjacencyLists[i].size; j++)
            {
                edge *e = g->adjacencyLists[i].list[j];
                dynarr_addLast(ret.adjacencyLists + i, createWeightedEdge(e->v1, e->v2, e->weight));
            }
        }
    }
    else
    {
        ret.adjacencyMatrix = malloc(g->n * sizeof(int *));
        for (int i = 0; i < g->n; i++)
        {
            ret.adjacencyMatrix[i] = malloc(g->n * sizeof(int));
            memcpy(ret.adjacencyMatrix[i], g->adjacencyMatrix[i], g->n * sizeof(int));
        }
    }

    return ret;
}

void graph_dfs(graph *g, int src, int *d, int *f, int *p, int *time)
{
    (*time)++;
    d[src] = *time;

    // discover neighbors
    if (g->adjacencyMode)
    {
        edge *e = NULL;
        dynarr_iterator it = dynarr_iterator_new(g->adjacencyLists + src);

        while ((e = dynarr_iterator_next(&it)))
        {
            if (e->weight && !d[e->v2])
            {
                // edge from src to i, i not discovered
                graph_dfs(g, e->v2, d, f, p, time);
                p[e->v2] = src;
            }
        }
    }
    else
    {
        for (int i = 0; i < g->n; i++)
        {
            if (g->adjacencyMatrix[src][i] && !d[i])
            {
                // edge from src to i, i not discovered
                graph_dfs(g, i, d, f, p, time);
                p[i] = src;
            }
        }
    }

    (*time)++;
    f[src] = *time;
}

void graph_dfsStart(graph *g, int *d, int *f, int *p)
{
    int time = 0;
    for (int i = 0; i < g->n; i++)
    {
        d[i] = 0;
        f[i] = 0;
        p[i] = -1;
    }

    for (int i = 0; i < g->noSources; i++)
    {
        graph_dfs(g, g->sources[i], d, f, p, &time);
    }
}

int graph_pathDfs(graph *g, int src, int dst, char *visited, int *p)
{
    if (!visited[src])
    {
        visited[src] = !0;

        if (g->adjacencyMode)
        {
            edge *e = NULL;
            dynarr_iterator it = dynarr_iterator_new(g->adjacencyLists + src);

            while ((e = dynarr_iterator_next(&it)))
            {
                if (e->weight && !visited[e->v2])
                {
                    // edge from src to i, i not discovered
                    if (e->v2 == dst)
                    {
                        // found goal
                        p[dst] = src;
                        visited[dst] = !0;
                        return e->weight;
                    }
                    else
                    {
                        int weight = graph_pathDfs(g, e->v2, dst, visited, p);
                        if (weight)
                        {
                            p[e->v2] = src;
                            // return smaller weight
                            return e->weight < weight ? e->weight : weight;
                        }
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < g->n; i++)
            {
                if (g->adjacencyMatrix[src][i] && !visited[i])
                {
                    if (i == dst)
                    {
                        // found goal
                        p[dst] = src;
                        visited[i] = !0;
                        return g->adjacencyMatrix[src][i];
                    }
                    else
                    {
                        int weight = graph_pathDfs(g, i, dst, visited, p);
                        if (weight)
                        {
                            p[i] = src;
                            // return smaller weight
                            return g->adjacencyMatrix[src][i] < weight ? g->adjacencyMatrix[src][i] : i;
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int graph_pathDfsStart(graph *g, int src, int dst, int *p)
{
    char *visited = malloc(g->n * sizeof(char));
    for (int i = 0; i < g->n; i++)
    {
        visited[i] = 0;
        p[i] = -1;
    }

    return graph_pathDfs(g, src, dst, visited, p);
}

typedef struct
{
    int *weight;
    int v;
} dijkstra_node;

int dijkstra_node_compare(void *v1, void *v2)
{
    dijkstra_node *n1 = v1;
    dijkstra_node *n2 = v2;

    int w1 = *((dijkstra_node *)v1)->weight;
    int w2 = *((dijkstra_node *)v2)->weight;

    if (w1 > w2)
    {
        return 1;
    }
    else if (w1 == w2)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

dijkstra_node *createDijkstraNode(int node, int *weight)
{
    dijkstra_node *ret = malloc(sizeof(dijkstra_node));

    ret->v = node;
    ret->weight = weight;

    return ret;
}

int indexFunction(void *v)
{
    return ((dijkstra_node *)v)->v;
}

#include "strstream.h"
char *printArr_(int *arr, int n)
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

char *printArr__(void **arr, int n)
{
    strstream ret = strstream_alloc(n);
    strstream_concat(&ret, "{ ");
    for (int i = 0; i < n; i++)
    {
        strstream_concat(&ret, "%d%s", ((dijkstra_node *)arr[i])->v, i == n - 1 ? " " : ", ");
    }

    strstream_concat(&ret, "}");

    return ret.str;
}

// algorithms
int *graph_dijkstra(graph *g, int src)
{
    int *d = malloc(g->n * sizeof(int));
    int *p = malloc(g->n * sizeof(int));

    minheap q = mheap_allocate(dijkstra_node_compare);

    d[src] = 0;
    p[src] = src;
    mheap_add(&q, createDijkstraNode(src, d + src));
    for (int i = 0; i < g->n; i++)
    {
        if (i == src)
        {
            continue;
        }
        d[i] = INT_MAX;
        p[i] = -1;
        mheap_add(&q, createDijkstraNode(i, d + i));
    }

    mheap_attachIndexMap(&q, indexFunction);

    dijkstra_node *cur = NULL;
    while (cur = mheap_pop(&q))
    {
        // relax
        if (g->adjacencyMode)
        {
            edge *e = NULL;
            dynarr_iterator it = dynarr_iterator_new(g->adjacencyLists + cur->v);

            while ((e = dynarr_iterator_next(&it)))
            {
                int v = e->v2;
                if (q.indexMap[v] != -1)
                {
                    // v not finished
                    // RELAX(cur, v, w)
                    if (d[v] > d[cur->v] + e->weight)
                    {
                        d[v] = d[cur->v] + e->weight;
                        p[v] = cur->v;
                        // DECREASE KEY
                        mheap_upheap(&q, q.indexMap[v]);
                    }
                }
            }
        }
        else
        {
            for (int v = 0; v < g->n; v++)
            {
                if (cur->v != v && g->adjacencyMatrix[cur->v][v] && q.indexMap[v] != -1)
                {
                    // edge exists from cur to v, v not finished
                    // RELAX(cur, v, w)
                    if (d[v] > d[cur->v] + g->adjacencyMatrix[cur->v][v])
                    {
                        d[v] = d[cur->v] + g->adjacencyMatrix[cur->v][v];
                        p[v] = cur->v;
                        // DECREASE KEY
                        mheap_upheap(&q, q.indexMap[v]);
                    }
                }
            }
        }

        free(cur);
    }

    mheap_freeDeep(&q);
    free(d);
    return p;
}

graph graph_fordFulkerson(graph *g, int srcIdx, int dst, int *maxFlowRet)
{
    graph ret = graph_copy(g);
    int maxFlow = 0;

    int src = g->sources[srcIdx];

    if (maxFlowRet)
    {
        *maxFlowRet = maxFlow;
    }

    return ret;
}