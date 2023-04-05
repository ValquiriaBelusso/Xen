#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define tempo 10 // testar com tempo de 10 e 30

// gcc -o teste3 teste3.c
// ./teste2

int *ptr;
long int x = 125 * 4 * 1024 * 1024;

int main()
{
      long int y = 8 * x;

      printf(" 0: %ld \n", y);
      fflush(stdout);
      ptr = calloc(1,x);
      sleep(tempo);
      free(ptr);
}
      