/* Four user fuctions were added to this libary, including:
atoi()int atoi(const char *s);  // converts string to integer 
int abs(int n);  // return absolute value of integer
void sleep(int seconds) // wrapper for sleep asm code
void wake // wrapper for wake asm code
*/

#include "stdlib.h"
#include "peachos.h"

char* itoa(int i)
{
    static char text[12];
    int loc = 11;
    text[11] = 0;
    char neg = 1;
    if (i >= 0)
    {
        neg = 0;
        i = -i;
    }

    while(i)
    {
        text[--loc] = '0' - (i % 10);
        i /= 10;
    }

    if (loc == 11)
        text[--loc] = '0';
    
    if (neg)
        text[--loc] = '-';

    return &text[loc];
}

int atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

int abs(int n){
    if (n < 0) {
        n = -n;
    }
    return n;
}

void* malloc(size_t size)
{
    return peachos_malloc(size);
}

void free(void* ptr)
{
    peachos_free(ptr);
}

void sleep(int seconds) {
    peachos_sleep(seconds);
}


void wake()
{
    peachos_wake();
}

