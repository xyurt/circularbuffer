#include "circularbuffer.h"

void    cb_init(CircularBuffer *cb, size_t capacity, size_t element_size) {
	if (cb == NULL) {
		return;
	}

	if (capacity > 0 && element_size > 0) {
		cb->data = (void *)CB_MALLOC(capacity * element_size);
		if (cb->data == NULL) {
			return;
		}

		cb->element_size = element_size;
		cb->capacity = capacity;
	}
	else {
		cb->capacity = 0;
		cb->element_size = 0;
		cb->data = NULL;
	}

	cb->head = 0;
	cb->tail = 0;
	cb->count = 0;
	
	return;
}
void    cb_free(CircularBuffer *cb) {
	if (cb == NULL || cb->data == NULL) {
		return;
	}

	CB_FREE(cb->data);
	cb->data = NULL;
	cb->capacity = 0;
	cb->element_size = 0;
	cb->head = 0;
	cb->tail = 0;
	cb->count = 0;

	return;
}

void    cb_resize(CircularBuffer *cb, size_t capacity) {
	void *new_block;
	size_t new_count;

	if (cb == NULL || cb->capacity == capacity) {
		return;
	}

	if (capacity == 0 || cb->element_size == 0) {
		cb_free(cb);
		return;
	}


	new_block = CB_MALLOC(capacity * cb->element_size);
	if (new_block == NULL) {
		return;
	}

	new_count = cb->count > capacity ? capacity : cb->count;
	if (new_count > 0) {
		size_t right = cb->capacity - cb->head;
		if (right > new_count) right = new_count;

		memcpy((char *)new_block, (char *)cb->data + cb->head * cb->element_size, right * cb->element_size);

		if (new_count > right) { 
			memcpy((char *)new_block + right * cb->element_size, (char *)cb->data, (new_count - right) * cb->element_size);
		}
	}

	CB_FREE(cb->data);
	cb->data = new_block;
	cb->capacity = capacity;
	cb->count = new_count;
	cb->head = 0;
	cb->tail = new_count;

	return;
}

void    cb_push(CircularBuffer *cb, void *element, int cb_push_type) {
	size_t new_capacity;

	if (cb == NULL) {
		return;
	}

	if (cb->count == cb->capacity) {
		switch (cb_push_type) {
			case 0: return; /* Reject    */
			case 1:         /* Overwrite */
			{
				cb->head = (cb->head + 1) % cb->capacity;
				cb->count--;
			} break;
			case 2:         /* Grow      */  
			{
				new_capacity = (cb->capacity == 0 ? 1 : cb->capacity * 2);
				cb_resize(cb, new_capacity);
				if (cb->data == NULL || cb->capacity != new_capacity) {
					return;
				}
			} break;
		}
		
	}

	void *dest = ((char *)cb->data) + (cb->tail * cb->element_size);
	if (cb->element_size != 0) {
		if (element == NULL) {
			memset(dest, 0, cb->element_size);
		}
		else {
			memcpy(dest, element, cb->element_size);
		}
	}

	cb->tail = (cb->tail + 1) % cb->capacity;
	cb->count++;

	return;
}

void    cb_push_reject(CircularBuffer *cb, void *element) {
	cb_push(cb, element, 0);
	return;
}
void    cb_push_overwrite(CircularBuffer *cb, void *element) {
	cb_push(cb, element, 1);
	return;
}
void    cb_push_grow(CircularBuffer *cb, void *element) {
	cb_push(cb, element, 2);
	return;
}

void   *cb_pop(CircularBuffer *cb) {
	void *result;

	if (cb == NULL || cb->count == 0) {
		return NULL;
	}

	result = (char *)cb->data + cb->head * cb->element_size;

	cb->head = (cb->head + 1) % cb->capacity;
	cb->count--;

	return result;
}
void   *cb_peek(const CircularBuffer *cb) {
	if (cb == NULL || cb->count == 0) {
		return NULL;
	}
	return (char *)cb->data + cb->head * cb->element_size;
}

void   *cb_get(const CircularBuffer *cb, size_t index) {
	if (cb == NULL || index >= cb->count) {
		return NULL;
	}
	return (char *)cb->data + ((cb->head + index) % cb->capacity) * cb->element_size;
}

void   cb_clear(CircularBuffer *cb) {
	if (cb == NULL) {
		return;
	}

	cb->head = 0;
	cb->tail = 0;
	cb->count = 0;
}