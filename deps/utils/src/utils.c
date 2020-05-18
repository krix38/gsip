#include <stdlib.h>
#include <stdio.h>
#include <utils.h>

void *allocate(size_t size)
{
        void *allocated = malloc(size);
        if (allocated == NULL)
        {
                fprintf(stderr, "memory allocation failure\n");
                exit(MEMORY_ALLOC_FAILURE);
        }
        return allocated;
}
