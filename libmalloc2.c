// Compilar para gerar a lib
// gcc -fPIC -shared -o libmalloc.so libmalloc.c -ldl
// Uso:
// LD_PRELOAD=./libmalloc.so ./<executavel_com_malloc_modif>

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
#include <xenstore.h>
// precisa instalar libxen-dev

static void *(*real_malloc)(size_t) = NULL;

static void mtrace_init(void)
{
    real_malloc = dlsym(RTLD_NEXT, "malloc");

    if (NULL == real_malloc)
    {
        fprintf(stderr, "Error in dlsym: %s", dlerror());
    }
}

void *malloc(size_t size)
{
    struct xs_handle *xs;
    xs_transaction_t th;
    int er;
    char *path;
    long int free_mem = 0;
    long int total_mem = sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE) / 1024;
    long int mem_alocada = size + total_mem;
    long int mem = total_mem - (total_mem * 0.8);

    // fprintf(stderr,"%ld", mem);

    if (real_malloc == NULL)
    {
        mtrace_init();
    }

    free_mem = sysconf(_SC_AVPHYS_PAGES) * sysconf(_SC_PAGESIZE) / 1024;

    fprintf(stderr, "--> malloc(%d) - memoria livre: %li memoria total: %li\n", size, free_mem, total_mem);

    if (free_mem <= mem)
    {
        /* Get a connection to the daemon */
        xs = xs_open(0);

        if (xs == NULL)
        {
            printf("erro conexao xs\n");
            exit(1);
        }

        // /* Get the local domain path */
        path = xs_get_domain_path(xs, 2); // alterar de 1 para o dom adequado

        if (path == NULL)
        {
            printf("erro get domain path\n");
            exit(1);
        }

        /* Make space for our node on the path */
        path = realloc(path, strlen(path) + strlen("/memory") + 1);
        if (path == NULL)
        {
            printf("erro alocacao path\n");
            exit(1);
        }

        strcat(path, "/memory");

        /* Create a watch on /local/domain/%d/mynode. */
        er = xs_watch(xs, path, "memory");

        if (er == 0)
        {
            printf("erro watch\n");
            exit(1);
        }

        /* Prepare a transaction and do a write. */
        th = xs_transaction_start(xs);
        er = xs_write(xs, th, path, "somestuff", strlen("somestuff"));
        xs_transaction_end(xs, th, false);
        if (er == 0)
        {
            printf("erro escrita\n");
            exit(1);
        }

        // char command[100];
        // sprintf(command, "sudo xenstore-write -s /local/domain/3/memory/target %ld", mem_alocada);
        // system(command);

        // fprintf(stderr, "%s", command);
    }

    void *p = real_malloc(size);

    return p;
}
