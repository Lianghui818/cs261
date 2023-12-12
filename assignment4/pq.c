/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Lianghui Wang
 * Email: wangl9@oregonstate.edu
 */

#include <stdlib.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq{
	struct dynarray* heap;
};

struct pq_element {
  	void* value;
  	int priority;
};

// help functions
void swap(struct dynarray* da, int i, int j) {
  	void* temp = dynarray_get(da, i);
  	dynarray_set(da, i, dynarray_get(da, j));
  	dynarray_set(da, j, temp);
}

void bubble_up(struct pq* pq, int index) {
  	if (index == 0) return;

	int parent_index = (index - 1) / 2;
	struct pq_element* child = dynarray_get(pq->heap, index);
	struct pq_element* parent = dynarray_get(pq->heap, parent_index);

	if (child->priority < parent->priority) {
		swap(pq->heap, index, parent_index);
		bubble_up(pq, parent_index);
	}
}

void bubble_down(struct pq* pq, int index) {
	int size = dynarray_size(pq->heap);
	int left_index = 2 * index + 1;
	int right_index = 2 * index + 2;
	int smallest_index = index;

	if (left_index < size) {
		struct pq_element* left_child = dynarray_get(pq->heap, left_index);
		struct pq_element* smallest = dynarray_get(pq->heap, smallest_index);
		if (left_child->priority < smallest->priority) {
		smallest_index = left_index;
		}
	}

	if (right_index < size) {
		struct pq_element* right_child = dynarray_get(pq->heap, right_index);
		struct pq_element* smallest = dynarray_get(pq->heap, smallest_index);
		if (right_child->priority < smallest->priority) {
		smallest_index = right_index;
		}
	}

	if (smallest_index != index) {
		swap(pq->heap, index, smallest_index);
		bubble_down(pq, smallest_index);
	}
}
// help function ends


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
	/*
	 * FIXME: 
	 */
	struct pq* pq = malloc(sizeof(struct pq));
	pq->heap = dynarray_create();
	return pq;
	}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	/*
	 * FIXME: 
	 */
	while (!pq_isempty(pq)) {
		struct pq_element* elem = pq_remove_first(pq);
		free(elem);
	}
	dynarray_free(pq->heap);
	free(pq);
		
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
	/*
	 * FIXME: 
	 */
	return dynarray_size(pq->heap) == 0;
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
	/*
	 * FIXME: 
	 */
	struct pq_element* new_element = malloc(sizeof(struct pq_element));
	new_element->value = value;
	new_element->priority = priority;
	dynarray_insert(pq->heap, new_element);
	bubble_up(pq, dynarray_size(pq->heap) - 1);
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
	/*
	 * FIXME: 
	 */
	if (pq_isempty(pq)) {
		return NULL;
	}
	struct pq_element* first_element = dynarray_get(pq->heap, 0);
	return first_element->value;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
	/*
	 * FIXME: 
	 */
	if (pq_isempty(pq)) {
		return -1;  // or some other invalid priority
	}
	struct pq_element* first_element = dynarray_get(pq->heap, 0);
	return first_element->priority;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
	/*
	 * FIXME: 
	 */
	if (pq_isempty(pq)) {
		return NULL;
	}

	// Get the first element
	struct pq_element* first_element = dynarray_get(pq->heap, 0);
	void* value = first_element->value;
	free(first_element);

	// Replace the first element with the last element
	int last_index = dynarray_size(pq->heap) - 1;
	if (last_index > 0) {
		struct pq_element* last_element = dynarray_get(pq->heap, last_index);
		dynarray_set(pq->heap, 0, last_element);
	}
	dynarray_remove(pq->heap, last_index);

	// Restore heap property
	if (!pq_isempty(pq)) {
		bubble_down(pq, 0);
	}

	return value;
}



