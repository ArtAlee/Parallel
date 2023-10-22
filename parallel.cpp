#include <stdio.h>
#include <limits.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

#define V 20000

int minKey(int key[], int visited[]) {
    int min = INT_MAX, index = -1, i;
#pragma omp parallel for shared(min, index) schedule(static)
    for (i = 0; i < V; i++) {
        if (visited[i] == 0 && key[i] < min) {
            #pragma omp critical
            {
                if (key[i] < min) {
                    min = key[i];
                    index = i;
                }
            }
        }
    }
    return index;
}

void printMST(int from[], int n, int graph) {
    int i;
    printf("Edge   Weight\n");
    for (i = 1; i < V; i++)
        printf("%d - %d    %d \n", from[i], i, graph[i][from[i]]);
}

void primMST(int graph) {
    int from[V];
    int key[V], num_threads;
    int visited[V];
    int i, count;

    for (i = 0; i < V; i++)
        key[i] = INT_MAX, visited[i] = 0;

    key[0] = 0;
    from[0] = -1;

    for (count = 0; count < V - 1; count++) {
        int u = minKey(key, visited);
        visited[u] = 1;

#pragma omp parallel for schedule(static)
        for (int v = 0; v < V; v++) {
            if (graph[u][v] && visited[v] == 0 && graph[u][v] < key[v]) {
                from[v] = u;
                key[v] = graph[u][v];
            }
        }
    }
}

int main() {
    int graph = (int )malloc(V * sizeof(int ));
    for (int x = 0; x < V; x++)
        graph[x] = (int)malloc(V * sizeof(int));

    int i, j;
    // Generate random adjacency matrix
    srand(time(NULL));
    for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
            graph[i][j] = rand() % 10;

    for (i = 0; i < V; i++)
        graph[i][i] = 0;

    for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
            graph[j][i] = graph[i][j];

    double start = omp_get_wtime();
    primMST(graph);
    double end = omp_get_wtime();
    printf("Time for parallel execution = %f\n", end - start);

    return 0;
}
