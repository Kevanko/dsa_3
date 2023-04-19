#pragma once

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double wtime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

typedef struct heapnode {
  int key;   /* Приоритет (ключ) */
  int value; /* Значение */
} heapnode;

typedef struct heap {
  int maxsize;            /* Максимальный размер кучи */
  int nnodes;             /* Число элементов */
  struct heapnode *nodes; /* Массив элементов. Для удобства реализации элементы
                             нумеруются с 1 */
} heap;
// Функция создания пустой бинарной кучи с указанной начальной вместимостью
struct heap *heap_create(int maxsize) {
  struct heap *h;
  h = (heap *)malloc(sizeof(*h));
  if (h != NULL) {
    h->maxsize = maxsize;
    h->nnodes = 0;
    h->nodes = (heapnode *)malloc(
        sizeof(*h->nodes) * (maxsize + 1)); /* Последний индекс - maxsize */
    if (h->nodes == NULL) {
      free(h);
      return NULL;
    }
  }
  return h;
}

void heap_free(struct heap *h) {
  free(h->nodes);
  free(h);
}

void heap_swap(struct heapnode *a, struct heapnode *b) {
  struct heapnode temp = *a;
  *a = *b;
  *b = temp;
}

struct heapnode *heap_min(struct heap *h) {
  if (h->nnodes == 0)
    return NULL;
  return &h->nodes[1];
}

int get_index(struct heap *h, int value) {
  for (int i = 1; i <= h->nnodes; i++) {
    {
      if (value == h->nodes[i].value)
        return i;
    }
  }
  return 0;
}

// Функция для вставки узла в бинарную кучу
int heap_insert(struct heap *h, int key, int value) {
  if (h->nnodes >= h->maxsize) /* Переполнение кучи */
    return -1;

  h->nnodes++;
  h->nodes[h->nnodes].key = key;
  h->nodes[h->nnodes].value = value;

  /* HeapifyUp */
  for (int i = h->nnodes; i > 1 && h->nodes[i].key < h->nodes[i / 2].key;
       i = i / 2)
    heap_swap(&h->nodes[i], &h->nodes[i / 2]);
  return 0;
}

// Функция, выполняющая процедуру heapify на указанной позиции в бинарной куче
void heap_heapify(struct heap *h, int index) {
  while (1) {
    int left = 2 * index;
    int right = 2 * index + 1;
    int smallest = index;
    if (left <= h->nnodes && h->nodes[left].key < h->nodes[smallest].key)
      smallest = left;
    if (right <= h->nnodes && h->nodes[right].key < h->nodes[smallest].key)
      smallest = right;
    if (smallest == index)
      break;
    heap_swap(&h->nodes[index], &h->nodes[smallest]);
    index = smallest;
  }
}

// Функция для извлечения минимального элемента из бинарной кучи
struct heapnode heap_extract_min(struct heap *h) {
  struct heapnode minnode = {
      0, 0}; // Изначально возвращаемый элемент с ключом и значением 0

  if (h->nnodes == 0)
    return minnode;

  minnode = h->nodes[1]; // Минимальный элемент находится в корне кучи
  h->nodes[1] =
      h->nodes[h->nnodes--]; // Заменяем корневой элемент последним элементом
  heap_heapify(h, 1); // Восстанавливаем свойство мин-кучи начиная с корня

  return minnode;
}

int heap_decrease_key(struct heap *h, int index, int newkey) {
  index = get_index(h, index);
  if (newkey >= h->nodes[index].key) {
    return -1;
  }

  h->nodes[index].key = newkey;
  while (index > 1 && h->nodes[index].key < h->nodes[index / 2].key) {
    heap_swap(&h->nodes[index], &h->nodes[index / 2]);
    index /= 2;
  }
  return index;
}
