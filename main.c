#include "cutils.h"

char *file_read2(const char *path, void*(alloc)(void*, size_t), void *ctx){
	size_t fsize = file_size(path);
	char *buffer = alloc(ctx, fsize);
	int fd = open(path, O_RDONLY);
	receive_data(fd, buffer, fsize);
	return buffer;
}

StringBuilder *sb_file_read_delim2(const char *path, const char *delim, void*(alloc)(void*, size_t), void *ctx){
	char *file_str = file_read2(path, alloc, ctx);
	StringBuilder *sb = alloc(ctx, sizeof(*sb));

	char *tok = strtok(file_str, delim);
	while(tok != NULL){
		da_push((*sb), tok);
		tok = strtok(NULL, delim);
	}
	return sb;
}

int main(void) {
	Arena a;
	StringBuilder *sb = sb_file_read_delim2("main.c", "\n", arena_alloc, &a);
	arena_free(&a);
}
