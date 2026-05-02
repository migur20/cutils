#include "cutils.h"

void arena_init(Arena *a, size_t size) {
  a->data = calloc(1, size);
  a->size = size;
  a->count = 0;
}

void *arena_alloc(Arena *a, size_t size){
  if (a->count + size <= a->size) {
    a->count += size;
    return a->data + a->count - size;
	}else 
		return NULL;
}

void *arena_alloc_buffer(Arena *a, const void *data, size_t size) {
  if (a->count + size <= a->size) {
    if (data != NULL)
      memcpy(a->data + a->count, data, size);
    a->count += size;
    return a->data + a->count - size;
  } else
    return NULL;
}

void arena_dump(Arena *a, char sep) {
  for (char *it = a->data; it < (char *)a->data + a->size; it++) {
    printf("%hhx%c", *it, sep);
  }
}

void arena_free(Arena *a){
	free(a->data);
	*a = (Arena){0};
}

int receive_data(int fd, void *buffer, size_t size) {
  size_t bytes_read = 0;
  while (bytes_read < size) {
    int n = read(fd, buffer + bytes_read, size - bytes_read);
    if (n <= 0)
      return EXIT_FAILURE;
    bytes_read += n;
  }
  return EXIT_SUCCESS;
}

void* arena_receive_data(int fd, Arena *a, size_t size) {
	char *buffer = arena_alloc(a, size);
	if(receive_data(fd, buffer, size) == EXIT_FAILURE)
		return NULL;
	return buffer;
}

int send_data(int fd, void *buffer, size_t size) {
  size_t bytes_written = 0;
  while (bytes_written < size) {
    int n = write(fd, buffer + bytes_written, size - bytes_written);
    if (n <= 0)
      return EXIT_FAILURE;
    bytes_written += n;
  }
  return EXIT_SUCCESS;
}

int send_block(int fd, void *buffer, uint32_t size) {
  if (send_data(fd, &size, sizeof(size)) == EXIT_FAILURE)
    return EXIT_FAILURE;
  return send_data(fd, buffer, size);
}

int create_inet_socket(int port) {
  int sockfd;
  struct sockaddr_in addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);

  int opt = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    fatal("bind");
  return sockfd;
}

size_t file_size(const char *path){
	struct stat s = {0};
	stat(path, &s);
	return s.st_size - 1;
}

char *arena_file_read(Arena *a, const char *path){
	size_t fsize = file_size(path);
	int fd = open(path, O_RDONLY);
	return arena_receive_data(fd, a, fsize);
}

char *file_read(const char *path){
	size_t fsize = file_size(path);
	char *buffer = calloc(1, fsize);
	int fd = open(path, O_RDONLY);
	receive_data(fd, buffer, fsize);
	return buffer;
}


StringBuilder *sb_file_read_delim(const char *path, const char* delim){
	char *file_str = file_read(path);
	StringBuilder *sb = malloc(sizeof(*sb));

	char *tok = strtok(file_str, delim);
	while(tok != NULL){
		da_push((*sb), tok);
		tok = strtok(NULL, delim);
	}
	return sb;
}

StringBuilder *sb_file_read(const char *path){
	return sb_file_read_delim(path, " \n\t");
}

void sb_dump(StringBuilder *sb, char* sep){
	da_foreach((*sb), char*, it){
		printf("%s%s", *it, sep);
	}
}

