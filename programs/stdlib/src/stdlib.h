#ifndef PEACHOS_STDLIB_H
#define PEACHOS_STDLIB_H
#include <stddef.h>

void* malloc(size_t size);
void free(void* ptr);
char* itoa(int i);
int atoi(const char *s);
void sleep(int seconds);
#endif