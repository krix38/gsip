#include <stdlib.h>
#include <stdio.h>

void *allocate(size_t size)
{
        void *allocated = malloc(size);
        if (allocated == NULL)
        {
                fprintf(stderr, "memory allocation failure\n");
                exit(1);
        }
        return allocated;
}
