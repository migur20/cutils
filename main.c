#include "cutils.h"

void sv_init(StringView *sv, const char *src) {
  sv->start = (char *)src;
  sv->size = strlen(src);
}

void sv_dump(StringView *sv) { printf("%.*s", sv->size, sv->start); }

void sv_trim_right(StringView *sv) {
  if (sv->size > 1)
    sv->size--;
}

void sv_trim_n_right(StringView *sv, int n) {
  for (int i = 0; i < n; i++)
    sv_trim_right(sv);
}

void sv_trim_left(StringView *sv) {
  sv->start++;
  sv_trim_right(sv);
}
void sv_trim_n_left(StringView *sv, int n) {
  for (int i = 0; i < n; i++)
    sv_trim_left(sv);
}

void sv_trim(StringView *sv) {
  while (isblank(*(sv->start)))
    sv_trim_left(sv);
  while (isblank(*(sv->start + sv->size)))
    sv_trim_right(sv);
}

int main(void) {
  const char *str = "  Hello, World!!!  ";
  StringView sv = {0};
  sv_init(&sv, str);

  putchar('\"');sv_dump(&sv);putchar('\"');
  putchar('\n');

  sv_trim(&sv);

  putchar('\"');sv_dump(&sv);putchar('\"');
}
