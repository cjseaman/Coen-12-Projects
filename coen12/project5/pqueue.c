/*
	Collin Seaman
	Project 5
	COEN 12
	
	This is an implementation of a priority queue using an array 
*/

# include <stdio.h>
# include <stdlib.h>
# include <assert.h>
# include <stdbool.h>
# include "pqueue.h"

# define START_LENGTH 10
# define p(x) (((x) - 1) / 2)
# define l(x) (((x) * 2) + 1)
# define r(x) (((x) * 2) + 2)

struct pqueue {
	int count;
	int size;
	void **data;
	int (*compare)();
};

/*
	Allocates and returns a pointer to a new empty queue of size START_LENGTH
	Also passes compare function to struct pqueue for later use
	O(1)
*/

PQ *createQueue(int (*compare)()) {
	
	PQ *pq = malloc(sizeof(PQ));
	assert(pq != NULL);
	pq->count = 0;
	pq->size = START_LENGTH;
	pq->data = malloc(sizeof(void*)*START_LENGTH);
	assert(pq->data != NULL);
	pq->compare = compare;	
	return pq;
}

/*
	Frees memory of all entries in queue and queue pointer
	Essentially deletes the queue from memeory
	O(1)
*/

void destroyQueue(PQ *pq) {

	assert(pq != NULL);
	free(pq->data);
	free(pq);
}

/*
	Returns pq->count so main can acess it
	O(1)
*/

int numEntries(PQ *pq) {
	
	assert(pq != NULL);
	return pq->count;
}

/*
	Adds an entry to the end of the queue and reorders the elements if order is broken
	First checks if there is enough memeory and allocates more if not
	O(logn)
*/
void addEntry(PQ *pq, void *entry) {

	assert(pq != NULL);
	int x = pq->count;
	int diff = 1;
	if(pq->count == pq->size) {
		pq->data = realloc(pq->data, sizeof(void*)*pq->size*2);
		pq->size *= 2;
	}
	pq->count++;
	pq->data[x] = entry;
/*
	Loop to correct unordered heap
*/
	while(x != 0) {
		diff = (*pq->compare)(pq->data[x], pq->data[p(x)]);
		if(diff > 0) {
			break;
		}
		pq->data[x] = pq->data[p(x)];
		pq->data[p(x)] = entry;
		x = p(x);
	}
}

/*
	Removes the first item from the queue, replacing it with the last value
	Then rebalances the heap and returns the deleted element
	O(logn)
*/
void *removeEntry(PQ *pq) {
	
	assert(pq != NULL);
	int x = pq->count-1;
	int diff;
	int min;
	void *deleted = pq->data[0];
	void *temp;
	pq->data[0] = pq->data[x];
	pq->count--;
	pq->data[x] = NULL;
	x = 0;
/*
	Loop to correct unordered heap
*/
	while(l(x) < pq->count) {
		
		min = l(x);
		if(r(x) < pq->count) {
			diff = (*pq->compare)(pq->data[l(x)], pq->data[r(x)]);
			if(diff > 0) {
				min = r(x);
			}
		}	
		temp = pq->data[x];
		diff = (*pq->compare)(pq->data[x], pq->data[min]);
		if(diff > 0) {
			pq->data[x] = pq->data[min];
			pq->data[min] = temp;
			x=min;
		}
		else {
			return deleted;
		}
	}
	return deleted;
	}
			

