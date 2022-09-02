//Compilar para gerar a lib
//gcc -fPIC -shared -o libmalloc.so libmalloc.c -ldl
//Uso:
//LD_PRELOAD=./libmalloc.so ./<executavel_com_malloc_modif>



#define _GNU_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h> 
#include <sys/sysinfo.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "libmalloc.h"

static void* (*real_malloc)(size_t)=NULL;


static void mtrace_init(void)
{
    real_malloc = dlsym(RTLD_NEXT, "malloc");
    
    if (NULL == real_malloc)
    {
        fprintf(stderr, "Error in dlsym: %s", dlerror());
    }
}

void *emalloc(size_t size)
{
    long int free_mem=0;
    long int total_mem=sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE)/1024;	
    long int mem_alocada= size + total_mem;
    long int mem = total_mem - (total_mem*0.8);
    
    //fprintf(stderr,"%ld", mem);
        
    if(real_malloc==NULL) {
        mtrace_init();
    }
   
    free_mem=sysconf(_SC_AVPHYS_PAGES) * sysconf(_SC_PAGESIZE)/1024;
    
    fprintf(stderr, "--> malloc(%d) - memoria livre: %li memoria total: %li\n", size,free_mem,total_mem);
    
    if(free_mem <= mem){
        char command[100];
        sprintf(command, "sudo xenstore-write -s /local/domain/3/memory/target %ld", mem_alocada);
        system(command);
        
        fprintf(stderr,"%s",command);
    }
 
    void *p = real_malloc(size);
    
   
    
    return p;
}

