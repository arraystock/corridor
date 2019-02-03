#ifndef STRING_H
#define STRING_H

#include <libk/stddef.h>

int memcmp(const void *, const void *, size_t);
void *memcpy(void *__restrict, const void *__restrict, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
void strcpy(char dest[], const char source[]);
size_t strlen(const char *);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, int n);

#endif
