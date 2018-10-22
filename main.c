#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "HeapADT.h"

typedef struct Heap_S{
    size_t capacity;
    int (*compFu) (const void * lhs, const void * rhs);
    void (*dumoEntry)(const void * item, FILE * outfp);
    const void ** array;
};

int parent(int i){
    return (i - 2)/2;
}

int right_child(int i){
    return (2*i)+1;
}

int left_child(int i){
    return (2*i)+2;
}

Heap create_empty_Heap(size_t capacity
        , int (*compFun)(const void * lhs, const void * rhs)
        , (*dumpEntry) (const void * item, FILE * outfp)){
    Heap new_heap = {capacity, compFun, dumpEntry};
    return new_heap;
}

void destroyHeap( Heap aHeap ){

}

size_t sizeHeap(Heap aHeap){
    return aHeap->capacity;
}

const void * topHeap(const Heap aHeap){
    return aHeap->array[0];
}

void * removeTopHeap(Heap aHeap){
    void * removed = aHeap->array[0];
    int last_idx = sizeof(aHeap->array)/ sizeof(aHeap->array[0]);
    memcpy(aHeap->array[0], aHeap->array[last_idx], sizeof(aHeap->array[0]));
    free(aHeap->array[last_idx]);
    int idx = 0;
    int idx_left = left_child(0);
    int idx_right = right_child(0);
    while((aHeap->compFu(aHeap->array[0], aHeap->array[idx_left]) > 0) ||
            (aHeap->compFu(aHeap->array[0], aHeap->array[idx_right]) > 0)){
        if(aHeap->compFu(aHeap->array[idx_right], aHeap->array[idx_left]) > 0){
            void * temp = malloc(sizeof(aHeap->array[0]));
            memcpy(temp, aHeap->array[idx_left], sizeof(aHeap->array[0]));
            memcpy(aHeap->array[idx_left], aHeap->array[idx], sizeof(aHeap->array[0]));
            memcpy(aHeap->array[idx], temp, sizeof(aHeap->array[0]));
            idx = idx_left;
            idx_left = left_child(idx);
            idx_right = left_child(idx);
            free(temp);
        } else{
            void * temp = malloc(sizeof(aHeap->array[0]));
            memcpy(temp, aHeap->array[idx_right], sizeof(aHeap->array[0]));
            memcpy(aHeap->array[idx_right], aHeap->array[idx], sizeof(aHeap->array[0]));
            memcpy(aHeap->array[idx], temp, sizeof(aHeap->array[0]));
            idx = idx_right;
            idx_left = left_child(idx);
            idx_right = left_child(idx);
            free(temp);
        }
    }
    return removed;
}

void insertHeapItem(Heap aHeap, const void * item){
    int idx = sizeof(aHeap->array)/ sizeof(item);
    aHeap->array = realloc(aHeap->array, sizeof(item));
    aHeap->array[idx] = item;
    int parent_indx = parent(idx);
    while (aHeap->compFu(aHeap->array[parent_indx], aHeap->array[idx]) > 0){
        // REMEMBER TO FREE THIS MEMORY
        memcpy(aHeap->array[idx], aHeap->array[parent_indx], sizeof(item));
        aHeap->array[parent_indx] = item;
        idx = parent_indx;
        parent_indx = parent(parent_indx);
    }
    aHeap->capacity ++;
}

void dumpHeap(Heap aHeap, FILE * outfp){
    //print entries in the correct order
}

int main() {
    printf("Hello, World!\n");
    return 0;
}