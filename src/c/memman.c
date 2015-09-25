/*-------------------------------------------------------------------------
 * memman.c
 *     wrappers around malloc/realloc/free
 *
 * Copyright (c) 2007-2015, glyn@8kb.co.uk
 * Author: Glyn Astill <glyn@8kb.co.uk>
 *
 *-------------------------------------------------------------------------
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
 * Wrappers around malloc/realloc/free
 */
void * wmalloc(unsigned int size) {
    char         *result;

    if ((result = malloc(size)) == NULL) {
        fprintf(stderr, "Failed to malloc %d bytes\n", size);
        exit(1);
    }
    return result;
}

void * wrealloc(void *iptr, unsigned int size) {
    char         *result;

    assert(iptr != NULL);

    if ((result = realloc(iptr, size)) == NULL) {
        fprintf(stderr, "Failed to realloc %d bytes\n", size);
        exit(1);
    }
    return result;
}

void wfree(void *iptr){
   assert(iptr != NULL);

    if (iptr) {
       free(iptr);
    }
    iptr = NULL;
}

/*
 * Reallocate memory block pointed to by iptr in chunks of chunk_size when
 * required_size is greater than value pointed to be allocated_size.
 * Sets value of allocated_size to current allocation.
 */
void * reallocate_block(void *iptr, int *allocated_size, int required_size, int chunk_size) {
    void        *result;

    if (*allocated_size >= required_size)
        return iptr;

    *allocated_size += (((required_size-*allocated_size)/chunk_size)+1)*chunk_size;

    result = wrealloc(iptr, *allocated_size);

    return result;
}
