#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "HeapADT.h"

struct Heap_S{
    size_t capacity;
    int size;
    int (*compFu) (const void * lhs, const void * rhs);
    void (*dumpEntry)(const void * item, FILE * outfp);
    const void ** array;
};

int parent(int i){
    return (i - 1)/2;
}

int left_child(int i){
    return 1 + i * 2;
}

int right_child(int i){
    return 2 + i * 2;
}

Heap createHeap(size_t capacity
        , int (*compFun)(const void * lhs, const void * rhs)
        , void (*dumpEntry) (const void * item, FILE * outfp)){
    Heap new_heap;
    new_heap = (Heap) malloc(sizeof(struct Heap_S));
    new_heap->size = 0;
    new_heap->capacity = capacity;
    new_heap->dumpEntry = dumpEntry;
    new_heap->compFu = compFun;
    return new_heap;
}

void destroyHeap( Heap aHeap ){
    for(int i = aHeap->size - 1 ; i >= 0 ; i--){
        free(aHeap->array[i]);
    }
    free(aHeap);
}

size_t sizeHeap(Heap aHeap){
    return aHeap->size;
}

const void * topHeap(const Heap aHeap){
    return aHeap->array[0];
}

void * removeTopHeap(Heap aHeap){
    void * removed = aHeap->array[0];
    int last_idx = aHeap->size -1;
    aHeap->array[0] = aHeap->array[last_idx];
    int idx = 0;
    int idx_left = left_child(0);
    int idx_right = right_child(0);
    while((idx_left < aHeap->size && idx_right < aHeap->size) && ((aHeap->compFu(aHeap->array[idx], aHeap->array[idx_left]) == 0) ||
            (aHeap->compFu(aHeap->array[idx], aHeap->array[idx_right]) == 0))){
        if(aHeap->compFu(aHeap->array[idx_right], aHeap->array[idx_left]) == 0){
            void * temp = aHeap->array[idx];
            aHeap->array[idx] = aHeap->array[idx_left];
            aHeap->array[idx_left] = temp;;
            idx = idx_left;
            idx_left = left_child(idx);
            idx_right = right_child(idx);
        } else{
            void * temp = aHeap->array[idx];
            aHeap->array[idx] = aHeap->array[idx_right];
            aHeap->array[idx_right] = temp;
            idx = idx_right;
            idx_left = left_child(idx);
            idx_right = right_child(idx);
        }
    }
    aHeap->size --;
    return removed;
}

void insertHeapItem(Heap aHeap, const void * item) {
    int idx = aHeap->size;
    if (idx == 0) {
        aHeap->array = malloc(sizeof(item));
    } else {
        if (idx == aHeap->capacity) {
            aHeap->capacity = 2 * aHeap->capacity;
        }
        aHeap->array = realloc(aHeap->array, sizeof(int));
    }
    aHeap->size++;
    aHeap->array[idx] = item;
    int parent_indx = parent(idx);
    while (idx != 0 && aHeap->compFu(aHeap->array[parent_indx], aHeap->array[idx]) == 0) {
        void * temp = aHeap->array[idx];
        aHeap->array[idx] = aHeap->array[parent_indx];
        aHeap->array[parent_indx] = temp;
        idx = parent_indx;
        parent_indx = parent(parent_indx);
    }
}


void dumpHeap(Heap aHeap, FILE * outfp){
    for(int i = 0 ; i < aHeap->size ; i++){
        aHeap->dumpEntry(aHeap->array[i], outfp);
    }
}

