#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // нужна для INT_MAX
#include <time.h>

#define MAX_VERTICES 10000 // максимальное количество вершин в графе
#define MAX_WEIGHT 1000 // максимальный вес ребра в графе
#define INF INT_MAX // представление бесконечности или очень большого числа, которое превышает любое возможное расстояние между вершинами графа.

// Структура под кучу
typedef struct {
    int size;
    int capacity;
    int *data;
    int *indices;
} MinHeap;

// создает новый граф с заданным количеством вершин. Выделяет память под матрицу смежности и заполняет ее бесконечностями (INF) и нулями по диагонали.
MinHeap *create_min_heap(int capacity) {
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->data = (int *)malloc(capacity * sizeof(int));
    heap->indices = (int *)malloc(capacity * sizeof(int));
    return heap;
}

// Вставка значения по индексу в бинарную кучу
void insert(MinHeap *heap, int value, int index) {
    int i = heap->size++;
    while (i && value < heap->data[(i - 1) / 2]) {
        heap->data[i] = heap->data[(i - 1) / 2];
        heap->indices[i] = heap->indices[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->data[i] = value;
    heap->indices[i] = index;
}

// Получение кратчайшего пути
int extract_min(MinHeap *heap) {
    int min_index = heap->indices[0];
    int last_value = heap->data[--heap->size];
    int last_index = heap->indices[heap->size];
    int i = 0, j = 1;
    while (j < heap->size) {
        if (j + 1 < heap->size && heap->data[j] > heap->data[j + 1]) j++;
        if (last_value <= heap->data[j]) break;
        heap->data[i] = heap->data[j];
        heap->indices[i] = heap->indices[j];
        i = j;
        j = 2 * j + 1;
    }
    heap->data[i] = last_value;
    heap->indices[i] = last_index;
    return min_index;
}

// Уменьшения значения ключа узла в бинарной куче. 
void decrease_key(MinHeap *heap, int index, int new_value) {
    for (int i = 0; i < heap->size; i++) {
        if (heap->indices[i] == index) {
            heap->data[i] = new_value;
            while (i && new_value < heap->data[(i - 1) / 2]) {
                int temp = heap->data[(i - 1) / 2];
                heap->data[(i - 1) / 2] = heap->data[i];
                heap->data[i] = temp;

                temp = heap->indices[(i - 1) / 2];
                heap->indices[(i - 1) / 2] = heap->indices[i];
                heap->indices[i] = temp;
                i = (i - 1) / 2;
            }
            break;
        }
    }
}

// Проверка кучи на пустоту
int is_empty(MinHeap *heap) {
    return heap->size == 0;
}

// Освобождение памяти, веделенную под кучу
void free_min_heap(MinHeap *heap) {
    free(heap->data);
    free(heap->indices);
    free(heap);
}

// Структура под графы
typedef struct {
    int **adj_matrix;
    int num_vertices;
} Graph;

// Функции для работы с графами
Graph *create_graph(int num_vertices) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;
    graph->adj_matrix = (int **)malloc(num_vertices * sizeof(int *));
        for (int i = 0; i < num_vertices; i++) {
        graph->adj_matrix[i] = (int *)malloc(num_vertices * sizeof(int));
        for (int j = 0; j < num_vertices; j++) {
            if (i == j) {
                graph->adj_matrix[i][j] = 0;
            } else {
                graph->adj_matrix[i][j] = INF;
            }
        }
    }
    return graph;
}

// Генерация связного графа из 20 вершин с случайными весами ребер.
void generate_connected_graph(Graph *graph) {
    for (int i = 0; i < graph->num_vertices - 1; i++) {
        for (int j = i + 1; j < graph->num_vertices; j++) {
            int weight = 1 + rand() % MAX_WEIGHT;
            graph->adj_matrix[i][j] = graph->adj_matrix[j][i] = weight;
        }
    }
}

// Генерация граф-решетки размерностью 100 × 100 вершин с случайными весами ребер. 
void generate_grid_graph(Graph *graph) {
    int grid_size = 100;
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            int vertex = i * grid_size + j;
            if (j + 1 < grid_size) {
                int weight = 1 + rand() % MAX_WEIGHT;
                graph->adj_matrix[vertex][vertex + 1] = weight;
                graph->adj_matrix[vertex + 1][vertex] = weight;
            }
            if (i + 1 < grid_size) {
                int weight = 1 + rand() % MAX_WEIGHT;
                graph->adj_matrix[vertex][vertex + grid_size] = weight;
                graph->adj_matrix[vertex + grid_size][vertex] = weight;
            }
        }
    }
}

// Алгоритм Дейкстра 
void dijkstra(Graph *graph, int start_vertex, int *distances) {
    MinHeap *heap = create_min_heap(graph->num_vertices);
    for (int i = 0; i < graph->num_vertices; i++) {
        distances[i] = INF;
        insert(heap, distances[i], i);
    }
    distances[start_vertex] = 0;
    decrease_key(heap, start_vertex, 0);

    while (!is_empty(heap)) {
        int current_vertex = extract_min(heap);
        for (int i = 0; i < graph->num_vertices; i++) {
            int weight = graph->adj_matrix[current_vertex][i];
            if (weight != INF) {
                int alt = distances[current_vertex] + weight;
                if (alt < distances[i]) {
                    distances[i] = alt;
                    decrease_key(heap, i, alt);
                }
            }
        }
    }

    free_min_heap(heap);
}

// Освобождение памяти, веделенную под графы
void free_graph(Graph *graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        free(graph->adj_matrix[i]);
    }
    free(graph->adj_matrix);
    free(graph);
}

// Вывод данных о коротких путях в файл
void print_shortest_paths(int *distances, int num_vertices, int start_vertex, FILE* file) {
    fprintf(file, "Кратчайшие пути от вершины %d:\n", start_vertex);
    for (int i = 0; i < num_vertices; i++) {
        fprintf(file, "До вершины %d: %d\n", i + 1, distances[i]);
    }
}

int main() {
    srand(time(NULL));

    FILE *file = fopen("result.txt", "w");
    if(file == NULL){
        return 1;
    }

    // Создание и анализ связного графа из 20 вершин
    int num_vertices = 20;
    Graph *connected_graph = create_graph(num_vertices);
    generate_connected_graph(connected_graph);

    int start_vertex = 0;
    int *distances_connected = (int *) malloc(num_vertices * sizeof(int));

    clock_t start_time = clock();
    dijkstra(connected_graph, start_vertex, distances_connected);
    clock_t end_time = clock();

    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Связный граф из 20 вершин:\n");

    print_shortest_paths(distances_connected, num_vertices, start_vertex + 1, file);
    printf("Время выполнения: %f секунд\n\n", execution_time);

     // Создание и анализ графа-решетки размерностью 100x100 вершин
    num_vertices = 10000;
    Graph *grid_graph = create_graph(num_vertices);
    generate_grid_graph(grid_graph);

    int *distances_grid = (int *) malloc(num_vertices * sizeof(int));

    start_time = clock();
    dijkstra(grid_graph, start_vertex, distances_grid);
    end_time = clock();

    execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Граф-решетка размерностью 100x100 вершин:\n");
    print_shortest_paths(distances_grid, num_vertices, start_vertex + 1, file);
    printf("Время выполнения: %f секунд\n", execution_time);

    free(distances_connected);
    free(distances_grid);
    free_graph(connected_graph);
    free_graph(grid_graph);

    return 0;
}