#include "dynamicarray.h"

typedef struct edge
{
    int v1;
    int v2;
    int weight;
} edge;
edge *createEdge(int v1, int v2);
edge *createWeightedEdge(int v1, int v2, int weight);

#define ADJ_MATRIX 0
#define ADJ_LIST !0

typedef struct graph
{
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