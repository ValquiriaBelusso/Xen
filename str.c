#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libmalloc.h>
#define STR_LEN 128

//gcc -I. -o str str.c libmalloc.c
// ./str

int main(int argc, const char *argv[])
{
        char *c;
        char *str1 = "Hello ";
        char *str2 = "World";

            
        //allocate an empty string
        c = emalloc(STR_LEN * sizeof(char));
        
        c[0] = 0x0;
        

        //and concatenate str{1,2}
        strcat(c, str1);
        strcat(c, str2);
        
        printf("New str: %s\n", c);


        return 0;
}
