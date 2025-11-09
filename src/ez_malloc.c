#include <assert.h>
#include <string.h>
#include "ez_malloc.h"


const int META_SIZE = sizeof(BlockMeta);
void* global_head = NULL; // Global head pointer for linked list.


BlockMeta *find_free_block(BlockMeta** last, size_t size) {
    BlockMeta* current = global_head;
    while(current && !(current->is_free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}


BlockMeta* request_space(BlockMeta* last, size_t size) {
    BlockMeta* block = sbrk(0);
    void* request = sbrk(size + META_SIZE);
    assert((void*)block == request); // Not thread safe.
    if(request == (void*)-1) return NULL; // sbrk failed.
    if(last) last->next = block; // NULL on first request.
    block->size = size;
    block->next = NULL;
    block->is_free = 0;
    return block;
}


BlockMeta* get_block_ptr(void* ptr) {
    return (BlockMeta*)ptr - 1;
}

/**
 * Takes as input he number of bytes and returns a pointer to a block of memory of that size.
 * 
 * 
 *  + -------------------- +
 *  |   ||  Stack          |
 *  |   \/  (grows down)   |
 *  + -------------------- +
 * 
 *  + ------------------------------ +
 *  |   ||  Memory Mapping Segment   |
 *  |   \/                           |
 *  + ------------------------------ +
 * 
 *  + ------------ +
 *  |   /\  Heap   |
 *  |   ||         |
 *  + ------------ +
 * 
 *  + ---------------------- +
 *  |   BSS segment          |
 *  + ---------------------- +
 *  |   Data segment         |
 *  + ---------------------- +
 *  |   Text segment (ELF)   |
 *  + ---------------------- +
 */
void* ez_malloc(size_t size) {
    // Simple malloc implementation:
    // malloc asks sbrk to increment the heap size and returns a pointer
    // to the start of the new region on the heap.
    /*
    void* ptr = sbrk(0); // Returns a pointer to the current top of the heap.
    void* request = sbrk(size); // Increments the heap size by 'size' and 
                                // returns a pointer to the previous top of the heap.
    if(request == (void*) -1) {
        return NULL; // sbrk failed.
    } else {
        assert(ptr == request); // Not thread safe.
        return ptr;
    }
     */

    if(size <= 0) return NULL;
    BlockMeta* block;
    if(!global_head) { // First call.
        block = request_space(NULL, size);
        if(!block) return NULL; // Failure.
        global_head = block;
    } else {
        BlockMeta* last = global_head;
        block = find_free_block(&last, size);
        if(!block) { // Failed to find free block.
            block = request_space(last, size);
            if(!block) return NULL;
        } else { // Found free block.
            block->is_free = 0;
        }
    }
    // Return a pointer to the region after BlockMeta, hence the +1.
    return block + 1; 
}


void* ez_realloc(void* ptr, size_t size) {
    // If no block pointer is provided, realloc behaves like malloc.
    if(!ptr) return ez_malloc(size);
    BlockMeta* block_ptr = get_block_ptr(ptr);
    // If the block already has enough space, then we don't need to add any more.
    // Could implement split to free old space.
    if(block_ptr->size >= size) return ptr;
    // Otherwise, the block doesn't have enough space and needs to tbe extended.
    // Need to malloc new block and copy the old contents into the new block.
    void* new_ptr = ez_malloc(size);
    if(!new_ptr) return NULL; // Failure.
    memcpy(new_ptr, ptr, block_ptr->size);
    ez_free(ptr);
    return new_ptr;
}


void* ez_calloc(size_t num_elem, size_t elem_size) {
    size_t size = num_elem * elem_size;
    void* ptr = ez_malloc(size);
    memset(ptr, 0, size); // Clear memory before returning.
    return ptr;
}


void ez_free(void* ptr) {
    if(!ptr) return;
    BlockMeta* block_ptr = get_block_ptr(ptr);
    assert(block_ptr->is_free == 0);
    block_ptr->is_free = 1;
}


size_t ez_get_size(void* ptr) {
    if(!ptr) return 0;
    BlockMeta* block_ptr = get_block_ptr(ptr);
    assert(block_ptr->is_free == 0);
    return block_ptr->size;
}
