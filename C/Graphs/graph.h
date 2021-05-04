#include "dynamicarray.h"

// typedef struct node
// {
//     int v;
//     dynamicarray neighbors;
// } node;

// int nodeCmp(void *n1, void *n2);
// int nodeValCmp(void *n1, void *n2);

typedef struct edge
{
    int v1;
    int v2;
    int weight;
} edge;
edge *createEdge(int v1, int v2);
edge *createWeightedEdge(int v1, int v2, int weight);

// int weightedEdgeCmp(void *e1, void *e2);

#define ADJ_MATRIX 0
#define ADJ_LIST !0

typedef struct graph
{
    //dynamicarray nodes;
    union
    {
        int **adjacencyMatrix;
        dynamicarray *adjacencyLists;
    };
    char adjacencyMode;

    int n;
    int noEdges;

    int *sources;
    int noSources;
} graph;

// node *graph_createNode(graph *g, int v);
// edge *graph_addEdge(graph *g, int v1, int v2);
// weightedEdge *graph_addWeightedEdge(graph *g, int v1, int v2, int weight);
// edge *graph_getEdge(graph *g, int v1, int v2);
// weightedEdge *graph_getWeightedEdge(graph *g, int v1, int v2);

graph graph_new(char mode, int n, int *sources, int noSources);
void graph_addVertex(graph *g);
void graph_addDirectedEdge(graph *g, int v1, int v2);
void graph_addDirectedWeightedEdge(graph *g, int v1, int v2, int weight);
void graph_addUndirectedEdge(graph *g, int v1, int v2);
void graph_addUndirectedWeightedEdge(graph *g, int v1, int v2, int weight);
void graph_free(graph *g);
graph graph_copy(graph *g);
char *graph_toString(graph *g);

// searching
void graph_dfs(graph *g, int src, int *d, int *f, int *p, int *time);
void graph_dfsStart(graph *g, int *d, int *f, int *p);
int graph_pathDfs(graph *g, int src, int dst, char *visited, int *p);
int graph_pathDfsStart(graph *g, int src, int dst, int *p);

// algorithms
int *graph_dijkstra(graph *g, int src);
graph graph_fordFulkerson(graph *g, int srcIdx, int dst, int *maxFlowRet);