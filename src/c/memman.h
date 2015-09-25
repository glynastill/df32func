/*-------------------------------------------------------------------------
 * memman.h
 *     definitions for wrappers around malloc/realloc/free
 *
 * Copyright (c) 2007-2015, glyn@8kb.co.uk
 * Author: Glyn Astill <glyn@8kb.co.uk>
 *
 *-------------------------------------------------------------------------
 */

#ifndef __MEMMAN_H__
#define __MEMMAN_H__

extern void * wmalloc(unsigned int size);
extern void * wrealloc(void *iptr, unsigned int size);
extern void wfree(void *iptr);
extern void * reallocate_block(void *iptr, int *allocated_size, int required_size, int chunk_size);

#endif
