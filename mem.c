#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MEGA 1<<20

void main()
{
    double *vet1;
    double *vet2;
    double *vet3;
    double *vet4;
    
    unsigned long int size;
    
    size = 50*MEGA;
    
    printf("-->%li\n", size);
    
    vet1=malloc(size*sizeof(double));
    for(int i=0;i<size;i++)
    {
        vet1[i]=1.0;
    }
    
    sleep(20);
    
    vet2=malloc(size*sizeof(double));
    for(int i=0;i<size;i++)
    {
        vet2[i]=1.0;
    }
    
    sleep(20);

    vet3=malloc(size*sizeof(double));
    for(int i=0;i<size;i++)
    {
        vet3[i]=1.0;
    }
    
    sleep(20);
    
    vet4=malloc(size*sizeof(double));
    for(int i=0;i<size;i++)
    {
        vet4[i]=1.0;
    }
    
    sleep(50);

}
