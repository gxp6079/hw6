
/*
 * file: HeapADT.c
 * @author: Giovana Puccini
 * @email: gxp6079@rit.edu
 * --------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "HeapADT.h"

/*
 * Heap Structure represents a generic heap
 */
struct Heap_S{
    size_t capacity;
    int size;
    int (*compFu) (const void * lhs, const void * rhs);
    void (*dumpEntry)(const void * item, FILE * outfp);
    const void ** array;
};

/*
 * parent gets parent of the index
 * @param i - index
 * @return the index of i's parent
 */
int parent(int i){
    return (i - 1)/2;
}

/*
 * left_child gets left_child of the index
 * @param i - index
 * @return the index of i's left child
 */
int left_child(int i){
    return 1 + i * 2;
}

/*
 * right_child gets left_child of the index
 * @param i - index
 * @return the index of i's right child
 */
int right_child(int i){
    return 2 + i * 2;
}

/*
 * creatHeap creates a Heap with the givem parameters
 * @param capacity
 * @param compFun - pointer to a comparision function
 * @param dumpEntry - pointer to a function that prints an entry
 * @return heap
 */
Heap createHeap(size_t capacity
                , int (*compFun)(const void * lhs, const void * rhs)
                , void (*dumpEntry) (const void * item, FILE * outfp)){
    Heap new_heap;
    new_heap = (Heap) malloc(sizeof(struct Heap_S));
    new_heap->size = 0;
    new_heap->capacity = capacity;
    new_heap->dumpEntry = dumpEntry;
    new_heap->compFu = compFun;
    new_heap->array = (void*) malloc(capacity * sizeof(void*));
    return new_heap;
}

/*
 * destroyHeap - free's all memory allocated for the heap
 * @param aHeap - Heap struct
 */
void destroyHeap( Heap aHeap ){
    free(aHeap->array);
    free(aHeap);
}

/*
 * sizeHeap - gives the Heap's current size
 * @param aHeap - Heap structure
 * @return size of the heap
 */
size_t sizeHeap(Heap aHeap){
    return aHeap->size;
}

/*
 * topHeap - gives the Heap's topmost element
 * @param aHeap - Heap structure
 * @return heap's top element
 */
const void * topHeap(const Heap aHeap){
    return aHeap->array[0];
}

/*
 * removeTopHeap - removes the top of the heap
 * @param aHeap - Heap structure
 * @return removed item
 */
void * removeTopHeap(Heap aHeap){
    void * removed = (void *) aHeap->array[0];
    int last_idx = aHeap->size -1;
    aHeap->array[0] = aHeap->array[last_idx];
    aHeap->size --;
    int idx = 0;
    int idx_left = left_child(0);
    int idx_right = right_child(0);
    while((idx_left < (int) aHeap->size && idx_right < (int) aHeap->size) && ((aHeap->compFu(aHeap->array[idx], aHeap->array[idx_left]) == 0) ||
                                                                              (aHeap->compFu(aHeap->array[idx], aHeap->array[idx_right]) == 0))){
        if(aHeap->compFu(aHeap->array[idx_right], aHeap->array[idx_left]) == 0){
            const void * temp = aHeap->array[idx];
            aHeap->array[idx] = aHeap->array[idx_left];
            aHeap->array[idx_left] = temp;;
            idx = idx_left;
            idx_left = left_child(idx);
            idx_right = right_child(idx);
        } else{
            const void * temp = aHeap->array[idx];
            aHeap->array[idx] = aHeap->array[idx_right];
            aHeap->array[idx_right] = temp;
            idx = idx_right;
            idx_left = left_child(idx);
            idx_right = right_child(idx);
        }
    }
    return removed;
}

/*
 * insertHeapItem - adds a new element to the heap
 * @param aHeap - Heap structure
 * @param item - a poiter to the item to be stored
 */
void insertHeapItem(Heap aHeap, const void * item) {
    int idx = aHeap->size;
    if (idx == aHeap->capacity) {
        aHeap->capacity = 2 * aHeap->capacity;
        aHeap->array = realloc(aHeap->array, aHeap->capacity * sizeof(void*));
    }
    aHeap->array[idx] = item;
    int parent_indx = parent(idx);
    while (idx != 0 && aHeap->compFu(aHeap->array[parent_indx], aHeap->array[idx]) == 0) {
        const void * temp = aHeap->array[idx];
        aHeap->array[idx] = aHeap->array[parent_indx];
        aHeap->array[parent_indx] = temp;
        idx = parent_indx;
        parent_indx = parent(parent_indx);
    }
    aHeap->size++;
}

/*
 * dumpHeap - prints the heap
 * @param aHeap - Heap structure
 * @param outfp - file where the heap will be printed
 */
void dumpHeap(Heap aHeap, FILE * outfp){
    for(int i = 0 ; i < aHeap->size ; i++){
        aHeap->dumpEntry(aHeap->array[i], outfp);
    }
}
