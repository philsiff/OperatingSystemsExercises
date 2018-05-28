#include "my_malloc.h"
#include <assert.h>
#include <stdio.h>
#include <sys/mman.h>

// A pointer to the head of the free list.
node_t *head = NULL;

// The heap function returns the head pointer to the free list. If the heap
// has not been allocated yet (head is NULL) it will use mmap to allocate
// a page of memory from the OS and initialize the first free node.
node_t *heap() {
    if (head == NULL) {
        // This allocates the heap and initializes the head node.
        head = (node_t *)mmap(NULL, HEAP_SIZE, PROT_READ | PROT_WRITE,
                MAP_ANON | MAP_PRIVATE, -1, 0);
        head->size = HEAP_SIZE - sizeof(node_t);
        head->next = NULL;
    }

    return head;
}

// Reallocates the heap.
void reset_heap() {
    if (head != NULL) {
        munmap(head, HEAP_SIZE);
        head = NULL;
        heap();
    }
}

// Returns a pointer to the head of the free list.
node_t *free_list() {
    return head;
}

// Calculates the amount of free memory available in the heap.
size_t available_memory() {
    size_t n = 0;
    node_t *p = heap();
    while (p != NULL) {
        n += p->size;
        p = p->next;
    }
    return n;
}

// Returns the number of nodes on the free list.
int number_of_free_nodes() {
    int count = 0;
    node_t *p = heap();
    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}

// Prints the free list. Useful for debugging purposes.
void print_free_list() {
    node_t *p = heap();
    while (p != NULL) {
        printf("Free(%zd)", p->size);
        p = p->next;
        if (p != NULL) {
            printf("->");
        }
    }
    printf("\n");
}

// Finds a node on the free list that has enough available memory to
// allocate to a calling program. This function uses the "first-fit"
// algorithm to locate a free node.
// 
// PARAMETERS:
// size - the number of bytes requested to allocate
//
// RETURNS:
// found - the node found on the free list with enough memory to allocate
// previous - the previous node to the found node
//
void find_free(size_t size, node_t **found, node_t **previous) {
    // TODO
    
    node_t *p1 = heap();
    node_t *p2 = NULL;
    while(p1 != NULL){
        if((p1->size + sizeof(node_t)) >= (size + sizeof(header_t))){
            *found = p1;
            *previous = p2;
            return;
        }else{
            p2 = p1;
            p1 = p1->next;
        }
    }
    *found = NULL;
    *previous = NULL;
}

// Splits a found free node to accommodate an allocation request.
//
// The job of this function is to take a given free_node found from
// `find_free` and split it according to the number of bytes to allocate.
// In doing so, it will adjust the size and next pointer of the `free_block`
// as well as the `previous` node to properly adjust the free list.
//
// PARAMETERS:
// size - the number of bytes requested to allocate
// previous - the previous node to the free block
// free_block - the node on the free list to allocate from
//
// RETURNS:
// allocated - an allocated block to be returned to the calling program
//  
void split(size_t size, node_t **previous, node_t **free_block, header_t **allocated) {
    assert(*free_block != NULL);
    
    node_t *old_block = *free_block;
    size_t actual_size = size + sizeof(header_t);
    *free_block = (node_t *)(((char *)*free_block) + actual_size);
    (*free_block)->size = old_block->size - (size + sizeof(header_t));
    (*free_block)->next = old_block->next;
    
    if(*previous == NULL){
        head = *free_block;
    }else{
        (*previous)->next = *free_block;
    }

    *allocated = (header_t *)old_block;
    (*allocated)->size = size;
    (*allocated)->magic = MAGIC;
}

// Returns a pointer to a region of memory having at least the request `size` bytes.
//
// PARAMETERS:
// size - the number of bytes requested to allocate
//
// RETURNS:
// A void pointer to the region of allocated memory
//
void *my_malloc(size_t size) {
    // TODO
    node_t *found;
    node_t *previous;
    header_t *allocated;
    
    find_free(size, &found, &previous);
    if(found == NULL){return NULL;}
    split(size, &previous, &found, &allocated);

    return (void *)(((char*)allocated) + sizeof(header_t));
}

// Merges adjacent nodes on the free list to reduce external fragmentation.
//
// This function will only coalesce nodes starting with `free_block`. It will
// not handle coalescing of previous nodes (we don't have previous pointers!).
//
// PARAMETERS:
// free_block - the starting node on the free list to coalesce
//
void coalesce(node_t *free_block) {
    // TODO
    size_t block_size = free_block->size + sizeof(node_t);
    
    node_t *curr_address = free_block;
    node_t *next_address = curr_address->next;

    while(next_address != NULL){ 
        if(((char *)curr_address) + block_size == (char *)next_address){
            curr_address->size += ((next_address->size) + sizeof(node_t)); 
            curr_address->next = next_address->next;
            next_address = next_address->next;
        }else{
            curr_address = curr_address->next;
            next_address = next_address->next;
        }
    }
}

// Frees a given region of memory back to the free list.
//
// PARAMETERS:
// allocated - a pointer to a region of memory previously allocated by my_malloc
//
void my_free(void *allocated) {
    // TODO
    header_t *actual_start = (header_t *)(((char *)allocated) - sizeof(header_t));
    size_t freed_bytes = actual_start->size + sizeof(header_t);
    
    assert(actual_start->magic == MAGIC);
    node_t *new_node = (node_t *)(actual_start);
    new_node->next = head;
    new_node->size = freed_bytes - sizeof(node_t);

    head = new_node;
    coalesce(head);
}
