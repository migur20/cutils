#include <stdlib.h>
#include <stdio.h>

#define da_define(Type, Name)                                                  \
  typedef struct {                                                             \
    Type *data;                                                                \
    size_t count;                                                              \
    size_t capacity;                                                           \
  } Name;

#define da_init(da)                                                      \
  do {                                                                         \
    da.count = 0;                                                              \
    da.capacity = 15;                                                          \
    da.data = calloc(da.capacity, sizeof(*(da.data)));                         \
  } while (0);

#define da_push(da, elem)                                                      \
  do {                                                                         \
    if (da.data == NULL)                                                       \
      da_init(da);                                                             \
    if (da.count >= da.capacity) {                                             \
      da.capacity *= 2;                                                        \
      da.data = realloc(da.data, da.capacity * sizeof(da.data[0]));            \
    }                                                                          \
    da.data[da.count++] = elem;                                                \
  } while (0);

#define da_destroy(da)                                                         \
  do {                                                                         \
    for (size_t i = 0; i < da.count; i++) {                                    \
      free(da.data[i]);                                                        \
    }                                                                          \
    free(da.data);                                                             \
  } while (0)

#define da_foreach(da, Type, it)                                               \
  for (Type *it = &da.data[0]; it != da.data + da.count; it++)

#define da_simple_sort(da, cmp, Type)                                          \
  do {                                                                         \
    int sorted = 0;                                                            \
    while (!sorted) {                                                          \
      sorted = 1;                                                              \
      for (size_t i = 0, j = 1; j < da.count; i++, j++) {                      \
        Type s1 = da.data[i];                                                  \
        Type s2 = da.data[j];                                                  \
        if (cmp(s1, s2) > 0) {                                                 \
          sorted = 0;                                                          \
          da.data[i] = s2;                                                     \
          da.data[j] = s1;                                                     \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  } while (0);
