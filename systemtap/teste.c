#include <stdio.h>
#include <stdlib.h>

// gcc -o teste teste.c

int *ptr;

int main()
{

      /* Initial memory allocation */
      printf("Malloc\n");
      fflush(stdout);
      ptr = (int *)malloc(1024 * 1024 * sizeof(int));

      for (int i = 0; i < 1024 * 1024; i++)
      {
            ptr[i] = i;
      }

      /* Reallocating memory */
      printf("Realloc \n");
      fflush(stdout);
      ptr = (int *)realloc(ptr, 1024 * 1024 * sizeof(int));

      printf("after alloc \n");

      for (int i = 0; i < 1024 * 1024; i++)
      {
            ptr[i] = i;
      }

      free(ptr);
}
