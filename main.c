#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#include "MinHeap.h"

#define MAX_VERTICES 20 // Максимальное количество вершин
#define MAX_VERTICES_GRID 4 * 4 // Максимальное количество вершин


void print_graph(int **graph, int size) {

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d,", graph[i][j]);
        }
        printf(" \n");
    }
    printf("\n");
}

void Dijekstra(struct heap* Q, int n,  int src, int **matrix, int D[], int prev[])
{
    int* H = (int*)calloc(n + 1, sizeof(int));
    H[src] = src;
    D[src] = 0;
    prev[src] = -1;
    heap_insert(Q, D[src], src);
    for (int i = 1; i < n + 1; i++)
    {
        if (i != src)
        {
            H[i] = 0;
            D[i] = INT_MAX;
            prev[i] = -1;
            heap_insert(Q, D[i], i);
        }
    }
    for (int i = 1; i < n + 1; i++)
    {
        //for (int ii = 1; ii <= Q->nnodes; ii++) {
            //printf("{%d, %d} ", h->nodes[ii].key, h->nodes[ii].value);
        //}
        struct heapnode v = heap_extract_min(Q);
        //printf(" -> {%d, %d}", v.key, v.value);
        //printf("\n");
        int vertex = v.value;
        H[vertex] = vertex;

        for (int j = 1; j < n + 1; j++)
        {
            if (matrix[vertex - 1][j - 1] && !H[j])
            {
                if (D[vertex] + matrix[vertex - 1][j - 1] < D[j])
                {
                    D[j] = D[vertex] + matrix[vertex - 1][j - 1];
                    heap_decrease_key(Q, j, D[j]);
                    prev[j] = vertex;
                }
            }
        }
    }
    free(H);
}


int main() {

    double time = 0.0f;

    int D[MAX_VERTICES + 1];
    int prev[MAX_VERTICES + 1];
    int src = 1;
    struct heap* Q = heap_create(MAX_VERTICES);

    int D_Grid[MAX_VERTICES_GRID + 1];
    int prev_Grid[MAX_VERTICES_GRID + 1];
    struct heap* Q_Grid = heap_create(MAX_VERTICES_GRID);

    int D_test[5 + 1];
    int prev_test[5 + 1];
    struct heap* Q_test = heap_create(5);

    /*/-->---->----->---->---->Генерация тестового графа<---<-----<----<-----<--/*/                                    
    ///                         0 | 10 |  0 | 30 | 100                          ///
    ///                        10 |  0 | 50 |  0 |   0                          ///
    ///                         0 | 50 |  0 | 20 |  10                          ///
    ///                        30 |  0 | 20 |  0 |  60                          ///
    ///                       100 |  0 | 10 | 60 |   0                          ///
    int** graph_test = (int**)calloc(5, sizeof(int*));                          ///
    if (!graph_test)                                                            ///
        return 0;                                                               ///
    for (int i = 0; i < 5; i++) {                                               ///
        graph_test[i] = (int*)calloc(5, sizeof(int));                           ///
        if (!graph_test[i])                                                     ///
            return 0;                                                           ///
    }                                                                           /// 
    graph_test[0][1] = 10; graph_test[0][3] = 30; graph_test[0][4] = 100;       ///   
    graph_test[1][0] = 10; graph_test[1][2] = 50;                               /// 
    graph_test[2][1] = 50; graph_test[2][3] = 20; graph_test[2][4] = 10;        /// 
    graph_test[3][0] = 30; graph_test[3][2] = 20; graph_test[3][4] = 60;        /// 
    graph_test[4][0] = 100; graph_test[4][2] = 10; graph_test[4][3] = 60;       /// 
    /*/-->---->----->---->---->Генерация тестового графа<---<-----<----<-----<--/*/

    time = wtime();
    Dijekstra(Q_test, 5, src, graph_test, D_test, prev_test);
    time = wtime() - time;


    printf("Test | vertex: 5\n D:\t");
    for (int i = 0; i < 5; i++) {
        printf("%2.1d ", D_test[i + 1]);
    }

    printf("\n prev:\t");
    for (int i = 0; i < 5; i++) {
        printf("%2.1d ", prev_test[i + 1]);
    }
    printf("\n time:\t %.12f\n\n", time);

    /*/-->---->----->---->---->Генерация ребер для связного графа<---<-----<----<-----<--/*/
    int** graph = (int**)calloc(MAX_VERTICES, sizeof(int*));                             ///
    if (!graph)                                                                          ///
        return 0;                                                                        ///
    for (int i = 0; i < MAX_VERTICES; i++) {                                             ///
        graph[i] = (int*)calloc(MAX_VERTICES, sizeof(int));                              ///
        if (!graph[i])                                                                   ///
            return 0;                                                                    ///
    }                                                                                    ///
                                                                                         ///
    for (int i = 0; i < MAX_VERTICES - 1; i++) {                                         ///
        graph[i][i + 1] = 1; /*/ Установка ребра между текущей и следующей вершинами /*/ ///
        graph[i + 1][i] = 1; /*/ Установка обратного ребра /*/                           ///
    }                                                                                    ///
                                                                                         ///                      
    graph[MAX_VERTICES - 1][0] = 1;                                                      ///
    graph[0][MAX_VERTICES - 1] = 1;                                                      ///
    /*/-->---->----->---->---->Генерация ребер для связного графа<---<-----<----<-----<--/*/

    time = wtime();
    Dijekstra(Q, MAX_VERTICES, src, graph, D, prev);
    time = wtime() - time;

    printf("Graph | vertex: %d\n D:\t", MAX_VERTICES);
    for (int i = 0; i < MAX_VERTICES; i++) {
        printf("%2.1d ", D[i + 1]);
    }
    printf("\n prev:\t");
    for (int i = 0; i < MAX_VERTICES; i++) {
        printf("%2.1d ", prev[i + 1]);
    }
    printf("\n time:\t %.12f\n\n", time);

    /*/-->---->----->---->---->Генерация ребер для  графа-решетки<---<-----<----<-----<--/*/
    int **graph_grid = (int**)calloc(MAX_VERTICES_GRID, sizeof(int*));                   ///
    if (!graph_grid)                                                                     ///
        return 0;                                                                        ///
    for (int i = 0; i < MAX_VERTICES_GRID; i++) {                                        ///
        graph_grid[i] = (int*)calloc(MAX_VERTICES_GRID, sizeof(int));                    ///
        if (!graph_grid[i])                                                              ///
            return 0;                                                                    ///
    }                                                                                    ///
                                                                                         ///    
    int delta = sqrt(MAX_VERTICES_GRID);                                                 ///
    for (int i = 0; i < MAX_VERTICES_GRID - 1; i++) {                                    ///
        int j = i + 1;                                                                   ///
        if ((i + 1) % delta != 0) {                                                      ///
            graph_grid[i][j] = 1;                                                        ///
            graph_grid[j][i] = 1;                                                        ///
        }                                                                                ///
        if ((i + delta) < MAX_VERTICES_GRID) {                                           ///
            graph_grid[i][i + delta] = 1;                                                ///
            graph_grid[i + delta][i] = 1;                                                ///
        }                                                                                ///
    }                                                                                    ///
    /*/-->---->----->---->---->Генерация ребер для  графа-решетки<---<-----<----<-----<--/*/

    time = wtime();
    Dijekstra(Q_Grid, MAX_VERTICES_GRID, src, graph_grid, D_Grid, prev_Grid);
    time = wtime() - time;

    printf("Graph_grid | vertex: %d\n D:\t", MAX_VERTICES_GRID);
    for (int i = 0; i < MAX_VERTICES_GRID; i++) {
        printf("%2.1d ", D_Grid[i + 1]);
    }
    printf("\n prev:\t");
    for (int i = 0; i < MAX_VERTICES_GRID; i++) {
        printf("%2.1d ", prev_Grid[i + 1]);
    }
    printf("\n time:\t %.12f\n\n", time);

    print_graph(graph_test, 5);
    print_graph(graph, MAX_VERTICES);
    print_graph(graph_grid, MAX_VERTICES_GRID);

    for (int i = 0; i < MAX_VERTICES; i++) {
        free(graph[i]);
    }
    for (int i = 0; i < MAX_VERTICES_GRID; i++) {
        free(graph_grid[i]);
    }
    for(int i = 0; i < 5; i++){
      free(graph_test[i]);
    }

    free(graph_test);
    free(graph_grid);
    free(graph);
    heap_free(Q);
    heap_free(Q_test);
    heap_free(Q_Grid);
    return 0;
}

