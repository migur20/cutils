#include "cutils.h"
#include <alloca.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
  void *key;
  void *data;
} Pair;

da_define(Pair, Map);

int cmp(void *d1, void *d2)
{
  return strcmp((char *)d1, (char *)d2);
}

void *map_get(Map *map, void *key, int(cmp)(void *, void *))
{
  da_foreach((*map), Pair, it)
  {
    if (cmp(key, it->key) == 0) {
      return it->data;
    }
  }
  return NULL;
}

void map_put(Map *map, void *key, void *data)
{
  Pair p = {
      .key = key,
      .data = data,
  };
  da_push((*map), p);
}

int read_line(int fd, char *buffer)
{
  int i = 0;
  char c;
  read(fd, &c, sizeof(c));
  while (c != '\n' && c != EOF) {
    buffer[i++] = c;
    read(fd, &c, sizeof(c));
  }
  return i;
}

da_define(char, String);

int str_append_line(int fd, String *s)
{
  int i = s->count;
  char c;
  read(fd, &c, sizeof(c));
  while (c != '\n' && c != EOF) {
    da_push((*s), c);
    read(fd, &c, sizeof(c));
  }
  return i;
}

int main(void)
{
  Map map = {0};
  int age = 20;
  map_put(&map, "foo", heap_alloc(&age, sizeof(age)));
  age++;
  map_put(&map, "bar", heap_alloc(&age, sizeof(age)));
  age++;
  map_put(&map, "baz", heap_alloc(&age, sizeof(age)));
  age++;

  da_foreach(map, Pair, it)
  {
    printf("%s, %d\n", (char *)it->key, *(int *)it->data);
  }

  Arena a = {0};
  arena_init(&a, 1048);
  char *s = arena_alloc(&a, 512);
  while (read_line(STDOUT_FILENO, s) != 0) {
    printf("%s\n", s);
  }
  arena_dump(&a, " ");
}
