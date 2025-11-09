#include <stdio.h>
#include <assert.h>
#include "ez_malloc.h"


void write_fib(int* output) {
    size_t size = ez_get_size(output);
    for(int i = 0; i < size / sizeof(int); ++i) {
        int val;
        if(i == 0) val = 0;
        else if(i == 1) val = 1;
        else val = output[i - 1] + output[i - 2];
        output[i] = val;
    }
}


void print_array(int* array) {
    size_t size = ez_get_size(array);
    printf("[");
    for(int i = 0; i < size / sizeof(int); ++i) {
        if(i != 0) printf(", ");
        printf("%d", array[i]);
    }
    printf("]\n");
}


int main() {
    // malloc new array of size 10 and write 10 fib.
    int* array = (int*)ez_malloc(10 * sizeof(int));
    assert(ez_get_size(array) == 10 * sizeof(int));
    write_fib(array);
    printf("Array of 10 fib (ez_malloc): ");
    print_array(array);

    // realloc array to size 20 and write 20 fib.
    array = (int*)ez_realloc(array, 20 * sizeof(int));
    assert(ez_get_size(array) == 20 * sizeof(int));
    write_fib(array);
    printf("Array of 10 fib (ez_realloc): ");
    print_array(array);

    ez_free(array);
    assert(get_block_ptr(array)->is_free == 1);

    return 0;
}