#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define tempo 10 // testar com tempo de 10 e 30

// gcc -o teste1 teste1.c
// ./teste1

int *ptr;
long int x = 125 * 4 * 1024 * 1024; // valor para fazer a alocacao de 500M

int main()
{
      int time = 0;
      for (int i = 1; i < 7; i++)
      {         
            printf(" %d: %ld \n", time, x * i);
            fflush(stdout);
            ptr = malloc(x * i);
            sleep(tempo);
            time = time + tempo;
            free(ptr);
      }
}
