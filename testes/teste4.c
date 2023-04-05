#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define tempo 10 // testar com tempo de 10 e 30

// gcc -o teste2 teste2.c
// ./teste2

int *ptr;

int main()
{
      srand(2);

      long int x = 50;

      printf(" 0: %ld \n", x);
      fflush(stdout);
      ptr = malloc(x);
      sleep(tempo);
      free(ptr);

      int time = 10;
      for (int i = 1; i < 6; i++)
      {
            x = x + (rand() % 300);

            printf(" %d: %ld \n", i, x);
            fflush(stdout);
            ptr = realloc(ptr, x);
            sleep(tempo);
            time = time + tempo;
            free(ptr);
      }
}
