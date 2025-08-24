#ifndef _CIRCULARBUFFER_H
#define _CIRCULARBUFFER_H

#include "stdlib.h"
#include "string.h"

#define CB_MALLOC  malloc
#define CB_REALLOC realloc
#define CB_FREE    free
#define CB_MEMCPY  memcpy

typedef struct CircularBuffer {
	void    *data;
	size_t  count;
	size_t  capacity;
	size_t  head;
	size_t  tail;
	size_t  element_size;
} CircularBuffer;

#define cb_is_full(cb) ((cb)->capacity) == ((cb)->count)
#define cb_is_empty(cb) (((cb)->count) == 0)

void    cb_init(CircularBuffer *cb, size_t capacity, size_t element_size);
void    cb_free(CircularBuffer *cb);

void    cb_resize(CircularBuffer *cb, size_t capacity);

void    cb_push_reject(CircularBuffer *cb, void *element);
void    cb_push_overwrite(CircularBuffer *cb, void *element);
void    cb_push_grow(CircularBuffer *cb, void *element);

void	*cb_pop(CircularBuffer *cb);
void	*cb_peek(const CircularBuffer *cb);

void	*cb_get(const CircularBuffer *cb, size_t index);

void    cb_clear(CircularBuffer *cb);

#endif /* _CIRCULARBUFFER_H */

