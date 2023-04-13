#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <unistd.h>

// gcc aloca.c -o aloca

int main(int argc, char *argv[])
{
      char command[100];
      size_t size = atoi(argv[1]);
      
	      long int free_mem;
	      long int total_mem;
	      long int mem_alocada;
	      long int mem;

	      free_mem = sysconf(_SC_AVPHYS_PAGES) * sysconf(_SC_PAGESIZE) / 1024;
	      total_mem = sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE) / 1024;
	      mem_alocada = size + (total_mem/1024); // valor em megabytes
	      mem = total_mem - (total_mem * 0.80);

	      //fprintf(stderr, "--> malloc(%li) - memoria livre: %li memoria total: %li mem: %li\n",  size,free_mem,total_mem,mem);
	      //fprintf(stderr, "--> memoria alocada(%li) - memoria alocada/1024: %li \n",  mem_alocada,mem_alocada/1024);

	      if (free_mem <= mem)
	      {
		    sprintf(command, "sudo xenstore-write -s /local/domain/3/memory/memalloc %ld", mem_alocada + 304); // lembrar de conferir o id
		    system(command);
		    printf("%s", command);
	      }
}
