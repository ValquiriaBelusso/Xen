#include <stdio.h>
#include <stdlib.h>

int *ptr;

int main(){

      printf("Before Alloc \n");
      fflush(stdout);
      ptr = (int*) malloc(1024*1024*sizeof(int));
      
      for  (int i= 0; i <1024*1024; i++){      
            ptr[i]=i;     
           // printf("%d", ptr[i]);
      }
      
      free(ptr);
}
