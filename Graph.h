#pragma once
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Graph {
  int nvertices; /*/    Число вершин       /*/
  int **m;       /*/    Матрица n x n      /*/
  int *D;        /*/   кратчайшие пути     /*/
  int *H;        /*/  посещенные вершины   /*/
  int *prev;     /*/ восстановление  пути  /*/
} Graph;

struct Graph *graph_create(int nvertices) {
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  graph->nvertices = nvertices;
  graph->m = (int **)malloc(nvertices * sizeof(int *));
  for (int i = 0; i < nvertices; i++) {
    graph->m[i] = (int *)calloc(nvertices, sizeof(int));
  }
  graph->D = (int *)calloc(nvertices + 1, sizeof(int));
  graph->H = (int *)calloc(nvertices + 1, sizeof(int));
  graph->prev = (int *)calloc(nvertices + 1, sizeof(int));
  return graph;
}

void graph_free(struct Graph *g) {
  free(g->m);
  free(g->H);
  free(g->D);
  free(g->prev);
  free(g);
}
