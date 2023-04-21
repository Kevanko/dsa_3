#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Graph.h"
#include "MinHeap.h"

#define MAX_VERTICES 20 // Максимальное количество вершин
#define MAX_VERTICES_GRID 100 * 100 // Максимальное количество вершин

/*/-->---->--->Генерация связного графа<---<--<----/*/
void generate_connected_graph(Graph *graph) {
  for (int i = 0; i < graph->nvertices - 1; i++) {
      int weight = 1 + rand() % 100;
      graph->m[i][i+1] = graph->m[i+1][i] = weight;
  }
  graph->m[0][graph->nvertices - 1] = graph->m[graph->nvertices - 1][0] = 1 + rand()%100;
}

/*/-->---->--->Генерация граф-решетки<---<--<----/*/
void generate_grid_graph(Graph *graph) {
  int grid_size = sqrt(graph->nvertices);
  for (int i = 0; i < grid_size; i++) {
    for (int j = 0; j < grid_size; j++) {
      int vertex = i * grid_size + j;
      int weight = 1 + rand() % 100;
      if (j + 1 < grid_size) {
        graph->m[vertex][vertex + 1] = weight;
        graph->m[vertex + 1][vertex] = weight;
      }
      if (i + 1 < grid_size) {
        graph->m[vertex][vertex + grid_size] = weight;
        graph->m[vertex + grid_size][vertex] = weight;
      }
    }
  }
}
void print_shortest(Graph *graph, int src){
	int tmp = src;
	int counter = 0;
	int *arr = (int*)calloc(sizeof(int), graph->nvertices);
	while(graph->prev[tmp] != -1){
		arr[graph->nvertices - counter - 1] = graph->prev[tmp];
		tmp = graph->prev[tmp];
		counter += 1;
	}
	for(int i = 0; i < graph->nvertices; i++){
	if (arr[i] != 0)
		printf("%d ", arr[i]);
	}
	printf("\n");
	free(arr);
};	

void print_graph(Graph *graph) {
for (int i = 0; i < graph->nvertices; i++) {
    for (int j = 0; j < graph->nvertices; j++) {
      printf("%d,", graph->m[i][j]);
    }
    printf(" \n");
  }
  printf("\n");
}

void Dijekstra(struct heap *Q, Graph *graph, int src) {
  graph->H[src] = src;
  graph->D[src] = 0;
  graph->prev[src] = -1;
  heap_insert(Q, graph->D[src], src);
  for (int i = 1; i < graph->nvertices + 1; i++) {
    if (i != src) {
      graph->H[i] = 0;
      graph->D[i] = INT_MAX;
      graph->prev[i] = -1;
      heap_insert(Q, graph->D[i], i);
    }
  }
  for (int i = 1; i < graph->nvertices + 1; i++) {
    // for (int ii = 1; ii <= Q->nnodes; ii++) {
    // printf("{%d, %d} ", h->nodes[ii].key, h->nodes[ii].value);
    //}
    struct heapnode v = heap_extract_min(Q);
    // printf(" -> {%d, %d}", v.key, v.value);
    // printf("\n");
    int vertex = v.value;
    graph->H[vertex] = vertex;

    for (int j = 1; j < graph->nvertices + 1; j++) {
      if (graph->m[vertex - 1][j - 1] && !graph->H[j]) {
        if (graph->D[vertex] + graph->m[vertex - 1][j - 1] < graph->D[j]) {
          graph->D[j] = graph->D[vertex] + graph->m[vertex - 1][j - 1];
          heap_decrease_key(Q, j, graph->D[j]);
          graph->prev[j] = vertex;
        }
      }
    }
  }
}

void get_result(struct heap *Q, Graph *graph, int src) {
  double timer = 0.0f;
  timer = wtime();
  Dijekstra(Q, graph, src);
  timer = wtime() - timer;


  printf("Graph | vertex: %d\n", graph->nvertices);
  printf("\n time:\t %.12f\n D:\t", timer);
  for (int i = 0; i < graph->nvertices && i < 20; i++) {
    printf("%2.1d ", graph->D[i + 1]);
  }

  printf("\n prev:\t");
  for (int i = 0; i < graph->nvertices && i < 20; i++) {
    printf("%2.1d ", graph->prev[i + 1]);
  }
  printf("\n");
}

int main() {
  int src = 1;
  struct heap *Q = heap_create(MAX_VERTICES > MAX_VERTICES_GRID? MAX_VERTICES : MAX_VERTICES_GRID);
  Graph *graph_test = graph_create(5);
  Graph *graph_connected = graph_create(MAX_VERTICES);
  Graph *graph_grid = graph_create(MAX_VERTICES_GRID);

  generate_connected_graph(graph_connected);
  generate_grid_graph(graph_grid);

  /*/-->---->----->---->---->Заполнение тестового графа<---<-----<----<-----<--/*/
  ///                         0 | 10 |  0 | 30 | 100                           ///
  ///                        10 |  0 | 50 |  0 |   0                           ///
  ///                         0 | 50 |  0 | 20 |  10                           ///
  ///                        30 |  0 | 20 |  0 |  60                           ///
  ///                       100 |  0 | 10 | 60 |   0                           ///
  /*/--->----->--->---->--->-->------->----<-----<----<----<---<---<------<----/*/
  graph_test->m[0][1] = 10;
  graph_test->m[0][3] = 30;
  graph_test->m[0][4] = 100;
  graph_test->m[1][0] = 10;
  graph_test->m[1][2] = 50;
  graph_test->m[2][1] = 50;
  graph_test->m[2][3] = 20;
  graph_test->m[2][4] = 10;
  graph_test->m[3][0] = 30;
  graph_test->m[3][2] = 20;
  graph_test->m[3][4] = 60;
  graph_test->m[4][0] = 100;
  graph_test->m[4][2] = 10;
  graph_test->m[4][3] = 60;
  

  /*/<---------> Print results <--------->/*/
  get_result(Q, graph_test, src);
  print_shortest(graph_test, 5);
  get_result(Q, graph_connected, src);
  get_result(Q, graph_grid, src);

  /*/<---------> Print matrix <--------->/*/
  print_graph(graph_test);
  print_graph(graph_connected);
  //print_graph(graph_grid);

  /*/<---------> Free <--------->/*/
  graph_free(graph_connected);
  graph_free(graph_grid);
  graph_free(graph_test);
  heap_free(Q);

  return 0;
}
||||||| dda716f
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Graph.h"
#include "MinHeap.h"

#define MAX_VERTICES 20 // Максимальное количество вершин
#define MAX_VERTICES_GRID 4 * 4 // Максимальное количество вершин

/*/-->---->--->Генерация связного графа<---<--<----/*/
void generate_connected_graph(Graph *graph) {
  int weight = 1;
  for (int i = 0; i < graph->nvertices - 1; i++) {
      graph->m[i][i+1] = graph->m[i+1][i] = weight;
  }
  graph->m[0][graph->nvertices - 1] = graph->m[graph->nvertices - 1][0] = 1;
}

/*/-->---->--->Генерация граф-решетки<---<--<----/*/
void generate_grid_graph(Graph *graph) {
  int grid_size = sqrt(graph->nvertices);
  for (int i = 0; i < grid_size; i++) {
    for (int j = 0; j < grid_size; j++) {
      int vertex = i * grid_size + j;
      if (j + 1 < grid_size) {
        int weight = 1;
        graph->m[vertex][vertex + 1] = weight;
        graph->m[vertex + 1][vertex] = weight;
      }
      if (i + 1 < grid_size) {
        int weight = 1;
        graph->m[vertex][vertex + grid_size] = weight;
        graph->m[vertex + grid_size][vertex] = weight;
      }
    }
  }
}

void print_graph(Graph *graph) {
for (int i = 0; i < graph->nvertices; i++) {
    for (int j = 0; j < graph->nvertices; j++) {
      printf("%d,", graph->m[i][j]);
    }
    printf(" \n");
  }
  printf("\n");
}

void Dijekstra(struct heap *Q, Graph *graph, int src) {
  graph->H[src] = src;
  graph->D[src] = 0;
  graph->prev[src] = -1;
  heap_insert(Q, graph->D[src], src);
  for (int i = 1; i < graph->nvertices + 1; i++) {
    if (i != src) {
      graph->H[i] = 0;
      graph->D[i] = INT_MAX;
      graph->prev[i] = -1;
      heap_insert(Q, graph->D[i], i);
    }
  }
  for (int i = 1; i < graph->nvertices + 1; i++) {
    // for (int ii = 1; ii <= Q->nnodes; ii++) {
    // printf("{%d, %d} ", h->nodes[ii].key, h->nodes[ii].value);
    //}
    struct heapnode v = heap_extract_min(Q);
    // printf(" -> {%d, %d}", v.key, v.value);
    // printf("\n");
    int vertex = v.value;
    graph->H[vertex] = vertex;

    for (int j = 1; j < graph->nvertices + 1; j++) {
      if (graph->m[vertex - 1][j - 1] && !graph->H[j]) {
        if (graph->D[vertex] + graph->m[vertex - 1][j - 1] < graph->D[j]) {
          graph->D[j] = graph->D[vertex] + graph->m[vertex - 1][j - 1];
          heap_decrease_key(Q, j, graph->D[j]);
          graph->prev[j] = vertex;
        }
      }
    }
  }
}

void get_result(struct heap *Q, Graph *graph, int src) {
  double timer = 0.0f;
  timer = wtime();
  Dijekstra(Q, graph, src);
  timer = wtime() - timer;

  printf("Graph | vertex: %d\n D:\t", graph->nvertices);
  for (int i = 0; i < graph->nvertices; i++) {
    printf("%2.1d ", graph->D[i + 1]);
  }

  printf("\n prev:\t");
  for (int i = 0; i < graph->nvertices; i++) {
    printf("%2.1d ", graph->prev[i + 1]);
  }
  printf("\n time:\t %.12f\n\n", timer);
}

int main() {
  int src = 1;
  struct heap *Q = heap_create(MAX_VERTICES > MAX_VERTICES_GRID? MAX_VERTICES : MAX_VERTICES_GRID);
  Graph *graph_test = graph_create(5);
  Graph *graph_connected = graph_create(MAX_VERTICES);
  Graph *graph_grid = graph_create(MAX_VERTICES_GRID);

  generate_connected_graph(graph_connected);
  generate_grid_graph(graph_grid);

  /*/-->---->----->---->---->Заполнение тестового графа<---<-----<----<-----<--/*/
  ///                         0 | 10 |  0 | 30 | 100                           ///
  ///                        10 |  0 | 50 |  0 |   0                           ///
  ///                         0 | 50 |  0 | 20 |  10                           ///
  ///                        30 |  0 | 20 |  0 |  60                           ///
  ///                       100 |  0 | 10 | 60 |   0                           ///
  /*/--->----->--->---->--->-->------->----<-----<----<----<---<---<------<----/*/
  graph_test->m[0][1] = 10;
  graph_test->m[0][3] = 30;
  graph_test->m[0][4] = 100;
  graph_test->m[1][0] = 10;
  graph_test->m[1][2] = 50;
  graph_test->m[2][1] = 50;
  graph_test->m[2][3] = 20;
  graph_test->m[2][4] = 10;
  graph_test->m[3][0] = 30;
  graph_test->m[3][2] = 20;
  graph_test->m[3][4] = 60;
  graph_test->m[4][0] = 100;
  graph_test->m[4][2] = 10;
  graph_test->m[4][3] = 60;
  

  /*/<---------> Print results <--------->/*/
  get_result(Q, graph_test, src);
  get_result(Q, graph_connected, src);
  get_result(Q, graph_grid, src);

  /*/<---------> Print matrix <--------->/*/
  print_graph(graph_test);
  print_graph(graph_connected);
  print_graph(graph_grid);

  /*/<---------> Free <--------->/*/
  graph_free(graph_connected);
  graph_free(graph_grid);
  graph_free(graph_test);
  heap_free(Q);

  return 0;
}
