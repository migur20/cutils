#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stddef.h>
#include <ctype.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#define fatal(msg)                                                             \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

typedef struct {
  void *data;
  size_t count;
  size_t size;
} Arena;

/*
 * Allocs an arena of <size> bytes
 * */
void arena_init(Arena *a, size_t size);
/*
 * Allocs <size> bytes of data in arena <a>
 * */
void *arena_alloc(void *_a, size_t size);
/*
 * Allocs <size> bytes of data in arena <a> and copies <data> data into it
 * */
void *arena_alloc_buffer(Arena *a, const void *data, size_t size);
/*
 * Prints the content of the arena as bytes with separator <sep>
 * */
void arena_dump(Arena *a, char sep);
/*
 * Frees Arena <a> data and sets all of its paramaters to 0 (NULL)
 * */
void arena_free(Arena *a);

#define da_define(Type, Name)                                                  \
  typedef struct {                                                             \
    Type *data;                                                                \
    size_t count;                                                              \
    size_t capacity;                                                           \
  } Name;

#define da_init(da)                                                            \
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

/*
 * Sends a block formated as: [4 bytes dimensão][dados]
 * */
int send_block(int fd, void *buffer, uint32_t size);

/*
 *
 * */
int create_inet_socket(int port);

/*
 * Receives size bytes from file descriptor <fd> into buffer
 * */
int receive_data(int fd, void *buffer, size_t size);

void *arena_receive_data(int fd, Arena *a, size_t size);

/*
 * Sends size bytes of buffer
 * */
int send_data(int fd, void *buffer, size_t size);

da_define(char *, StringBuilder);

void sb_dump(StringBuilder *sb, char *sep);

StringBuilder *sb_file_read(const char *path);

StringBuilder *sb_file_read_delim(const char *path, const char *delim);

char *file_read(const char *path);

char *arena_file_read(Arena *a, const char *path);

size_t file_size(const char *path);

typedef struct{
	char *start;
	int size;
}StringView;

void sv_init(StringView *sv, const char *src);

void sv_dump(StringView *sv);

void sv_trim_right(StringView *sv);

void sv_trim_n_right(StringView *sv, int n);

void sv_trim_left(StringView *sv);

void sv_trim_n_left(StringView *sv, int n);

void sv_trim(StringView *sv);
