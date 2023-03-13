#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <unistd.h>

static void* (*real_malloc)(size_t)         = NULL;
static void* (*real_realloc)(void*, size_t) = NULL;
static void* (*real_calloc)(size_t, size_t) = NULL;
static void  (*real_free)(void*)            = NULL;

static int alloc_init_pending = 0;


/* Load original allocation routines at first use */
static void alloc_init(void)
{
  alloc_init_pending = 1;
  real_malloc  = dlsym(RTLD_NEXT, "malloc");
  real_realloc = dlsym(RTLD_NEXT, "realloc");
  real_calloc  = dlsym(RTLD_NEXT, "calloc");
  real_free    = dlsym(RTLD_NEXT, "free");
  if (!real_malloc || !real_realloc || !real_calloc || !real_free) {
    fputs("libmalloc.so: Unable to hook allocation!\n", stderr);
    fputs(dlerror(), stderr);
    exit(1);
  }
  //else {
    //fputs("libmalloc.so: Successfully hooked\n", stderr);
  //}
  alloc_init_pending = 0;
}

#define ZALLOC_MAX 1024
static void* zalloc_list[ZALLOC_MAX];
static size_t zalloc_cnt = 0;

/* dlsym needs dynamic memory before we can resolve the real memory 
 * allocator routines. To support this, we offer simple mmap-based 
 * allocation during alloc_init_pending. 
 * We support a max. of ZALLOC_MAX allocations.
 */
void* zalloc_internal(size_t size)
{
  fputs("alloc.so: zalloc_internal called", stderr);
  if (zalloc_cnt >= ZALLOC_MAX-1) {
    fputs("alloc.so: Out of internal memory\n", stderr);
    return NULL;
  }
  /* Anonymous mapping ensures that pages are zero'd */
  void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
  if (MAP_FAILED == ptr) {
    perror("alloc.so: zalloc_internal mmap failed");
    return NULL;
  }
  zalloc_list[zalloc_cnt++] = ptr; /* keep track for later calls to free */
  return ptr;
}


//**********
//**FREE**
//**********
void free(void* ptr)
{
  if (alloc_init_pending) {
    fputs("alloc.so: free internal\n", stderr);
    /* Ignore 'free' during initialization and ignore potential mem leaks 
     * On the tested system, this did not happen
     */
    return;
  }
  
  if(!real_malloc) {
    alloc_init();
  }
  for (size_t i = 0; i < zalloc_cnt; i++) {
    if (zalloc_list[i] == ptr) {
      /* If dlsym cleans up its dynamic memory allocated with zalloc_internal,
       * we intercept and ignore it, as well as the resulting mem leaks.
       * On the tested system, this did not happen
       */
      return;
    }
  }
  real_free(ptr);
}


//**********
//**MALLOC**
//**********
void *malloc(size_t size)
{
  char command[100];
      
  if(size>1024)
  {
      
      if (alloc_init_pending) {
        fputs("alloc.so: malloc internal\n", stderr);
        return zalloc_internal(size);
      }
      
      if(!real_malloc) {
        alloc_init();
        
      }
      void* result = real_malloc(size);
      long int free_mem=0;
      long int total_mem=sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE)/1024;	
      long int mem_alocada= (size/1024) + total_mem+300000;
      long int mem = total_mem - (total_mem*0.60);
      
      free_mem=sysconf(_SC_AVPHYS_PAGES) * sysconf(_SC_PAGESIZE)/1024;
    
      if(real_malloc==NULL) {
        alloc_init();
      }
   
      fprintf(stderr, "--> malloc(%li) - memoria livre: %li memoria total: %li\n",  size,free_mem,total_mem);
      
      
      if(free_mem <= mem){
      	fprintf(command, "sudo xenstore-write -s /local/domain/2/memory/memalloc %ld", mem_alocada/1024);
      	system(command);
      }
      return result;
  }
  else
  {
     // write(1, "++++++\n", 7);  
      if(!real_malloc) {
        alloc_init();
      }
      void* result = real_malloc(size);
      return result;
  }
      
}




//**********
//**REALLOC**
//**********
void *realloc(void* ptr, size_t size)
{

  char command[100];

  if(size>1024){

    if (alloc_init_pending) {
      fputs("alloc.so: realloc internal\n", stderr);
      if (ptr) {
        fputs("alloc.so: realloc resizing not supported\n", stderr);
        exit(1);
      }
      return zalloc_internal(size);
    }

    if(!real_malloc) {
      alloc_init();
    }
    
    void* result = real_realloc(ptr, size);
      	fprintf(stderr, "2realloc(%ld)\n", size);
      	//fflush(stderr);
      	
        long int free_mem=0;
        long int total_mem=sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE)/1024;	
        long int mem_alocada= (size/1024) + total_mem+300000;
        long int mem = total_mem - (total_mem*0.60);
      
        free_mem=sysconf(_SC_AVPHYS_PAGES) * sysconf(_SC_PAGESIZE)/1024;
    
      	if(real_realloc==NULL) {
          alloc_init();
      	}
   
        fprintf(stderr, "--> realloc(%li) - memoria livre: %li memoria total: %li\n", size,free_mem,total_mem);
      
      
      if(free_mem <= mem){
      	write(1, "-----\n", 7);  
      	sprintf(command, "sudo xenstore-write -s /local/domain/2/memory/memalloc %ld", mem_alocada/1024);
      	system(command);
      }

  }

  return real_realloc(ptr, size);
}





//**********
//**CALLOC**
//**********
void *calloc(size_t nmemb, size_t size)
{

  size_t newsize;
  newsize = nmemb * size;
  
  char command[100];
  
  fprintf(stderr, "1calloc(%ld)\n", newsize);
  
  if(newsize>1024){
  
  	if (alloc_init_pending) {
    	fputs("alloc.so: calloc internal\n", stderr);
    	/* Be aware of integer overflow in nmemb*size.
     	* Can only be triggered by dlsym */
    	return zalloc_internal(nmemb * size);
  	}
  	
  	if(!real_calloc) {
    	alloc_init();
  	}
  
  	void* result = real_calloc(nmemb, size);
      	fprintf(stderr, "2calloc(%ld)\n", newsize);
      	//fflush(stderr);
      	
        long int free_mem=0;
        long int total_mem=sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE)/1024;	
        long int mem_alocada= (newsize/1024) + total_mem+300000;
        long int mem = total_mem - (total_mem*0.60);
      
        free_mem=sysconf(_SC_AVPHYS_PAGES) * sysconf(_SC_PAGESIZE)/1024;
    
      	if(real_calloc==NULL) {
          alloc_init();
      	}
   
        fprintf(stderr, "--> calloc(%li) - memoria livre: %li memoria total: %li\n", size,free_mem,total_mem);
      
      
      if(free_mem <= mem){
      	write(1, "-----\n", 7);  
      	sprintf(command, "sudo xenstore-write -s /local/domain/2/memory/memalloc %ld", mem_alocada/1024);
      	system(command);
      }
      
  }
  return real_calloc(nmemb, size);
}


