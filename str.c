#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define STR_LEN 1280

#define K 1024
#define tempo 20

//gcc -I. -o str str.c 
// ./str

void main()
{
    char *vet1;
    /*char *vet2;
    char *vet3;
    char *vet4;*/
    
    unsigned long int size;
    
    size = 80*K;
    
    //printf("-->%li\n", size);
    
    vet1=malloc(size*sizeof(char));
       for(int i=0;i<size;i++)
    {
        vet1[i]=1;
    }

   /*sleep(tempo);
    
    vet2=malloc(size*sizeof(char));
    for(int i=0;i<size;i++)
    {
        vet2[i]=1;
    }
    
    sleep(tempo);

    vet3=malloc(size*sizeof(char));
    for(int i=0;i<size;i++)
    {
        vet3[i]=1;
    }
    
    sleep(tempo);
    
    vet4=malloc(size*sizeof(char));
    for(int i=0;i<size;i++)
    {
        vet4[i]=1;
    }
    
    sleep(tempo);*/
}
