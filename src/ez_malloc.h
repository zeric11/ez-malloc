#ifndef EZ_MALLOC_H
#define EZ_MALLOC_H

#include <sys/types.h>
#include <unistd.h>


typedef struct BlockMeta {
    size_t size; // in bytes
    struct BlockMeta *next;
    int is_free; // bool
} BlockMeta;

extern const int META_SIZE;
extern void* global_head; // Global head pointer for linked list.

BlockMeta* find_free_block(BlockMeta** last, size_t size);
BlockMeta* request_space(BlockMeta* last, size_t size);
BlockMeta* get_block_ptr(void* ptr);

/**
 * Takes as input he number of bytes and returns a pointer to a block of memory of that size.
 * Uses the sbrk system call which is only available in Unix-based systems.
 */
void* ez_malloc(size_t size);
void* ez_realloc(void* ptr, size_t size);
void* ez_calloc(size_t num_elem, size_t elem_size);
void ez_free(void *ptr);
size_t ez_get_size(void* ptr);


#endif /* EZ_MALLOC_H */