/*
	Collin Seaman
	Atkinson COEN 12 
	Project 4: Circular Doubly Linked List

	This is an implementation of a circular doubly linked list which supports insertion and deletion from both ends. 
	The list contains data in the form of void pointers which allow us to manipulate it without caring about its type.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "list.h"

struct node {			//holds data
	struct node *next;	//next node
	struct node *previous;	//prev node
	void *data;		//data
};

struct list {			//holds information about list
	int count;		//number of items in list
	struct node *head;	//head of list (dummy node)
	int (*compare)();	//compare function
};
	
#define NODE struct node

LIST *createList(int (*compare)()) {	//creates an empty list O(1)
	LIST *listPoint;		//list pointer
	NODE *dummyNode;		//dummy node pointer
	listPoint = malloc(sizeof(LIST));
	assert(listPoint != NULL);
	listPoint->count = 0;		//count is zero
	listPoint->compare = compare;	//store compare function
	dummyNode = malloc(sizeof(NODE));
	listPoint->head=dummyNode;	//assign head a dummy value
	dummyNode->next=dummyNode;	//next and previous point to head
	dummyNode->previous=dummyNode;
	return listPoint;		//return pointer to new list
}

void destroyList(LIST *lp) {		//destroys list O(n)
	NODE *p = lp->head->next;	//initialize p at head->next
	while (p != lp->head) {		//traverse list until dummy node
		p = p->next;
		free(p->previous);	//free previous node
	}
	free(p);			//free dummy node
	free(lp);			//free list pointer
}

int numItems(LIST *lp) {	//returns number of elements in list O(1)
	return lp->count;	
}

void addFirst(LIST *lp, void *item) {	//adds element at start of list O(1)
	NODE *p = malloc(sizeof(NODE));	
	assert(p != NULL);		
	
	lp->head->next->previous = p;	//old first element now points back at new one
	p->next = lp->head->next;	//new first element points forward at old one
	p->previous = lp->head;		//new points back at head
	lp->head->next = p;		//head points forward at new
	
	lp->count++;			//increment count!!
	
	p->data = item;			//return new element
}

void addLast(LIST *lp, void *item) {	//adds element at end of list O(1)
	NODE *p = malloc(sizeof(NODE));
	assert(p != NULL);
	
	p->previous = lp->head->previous;	//new element points back at old last element
	lp->head->previous = p;			//head points back at new last element
	p->previous->next = p;			//old element points forward at new
	p->next = lp->head;			//new element points forward at head
	lp->count++;				//increment count

	p->data = item;	
}

void *removeFirst(LIST *lp) {			//remove item from start of list and return data O(1)
	void *item;
	NODE *first;
	if (lp->head->next == lp->head) {	//if list is empty, return NULL
		return NULL;
	}
	first = lp->head->next;			//save address
	item = first->data;			//item is data of first
	first->next->previous = lp->head;	//reset previous of next pointer
	lp->head->next = first->next;		//set head to point after first
	
	lp->count--;				//DECREMENT COUNT!!

	free(first);				//be free my child
	return item;
}

void *removeLast(LIST *lp) {			//remove and return item from the end of the list O(1)
	void *item;
	NODE *last;
	if (lp->head->previous == lp->head) {
		return NULL;			//if list is empty return NULL
	}
	last = lp->head->previous;		//save address
	item = last->data;			//set value of item
	lp->head->previous = last->previous;	//head previous points to last previous
	last->previous->next = lp->head;	//head previous points forward at head
	
	lp->count--;				//DECREMENT COUNT!!
	
	free(last);				//be free my child
	return item;				
}

void *getFirst(LIST *lp) {			//return the first item's data in the list O(1)
	return lp->head->next->data;
}

void *getLast(LIST *lp) {			//return the last item's data in the list O(1)
	return lp->head->previous->data;
}

void removeItem(LIST *lp, void *item) {		//remove item with the given data O(n)
	NODE *p = lp->head;
	while (p->next != lp->head) {		//loop to traverse list
		p=p->next;
		if (lp->compare(p->data, item) == 0) {	//use compare function, deallocate if equal
			p->previous->next = p->next;
			p->next->previous = p->previous;
			free(p);
			lp->count--;
			return;
		}
	}
}

void *findItem(LIST *lp, void *item) {		//return the data of an item if it is found or NULL if not O(n)
	NODE *p = lp->head;
	assert(lp->compare != NULL);
	while (p->next != lp->head) {		//loop to traverse list
		p=p->next;
		if (lp->compare(p->data, item) == 0) {	//use compare function, return data if equal
			return p->data;
		}
	}
	return NULL;	//else return NULL
}

void *getItems(LIST *lp) {			//return an array of pointers to the data in the list O(n)
	int i;
	void **array;
	NODE *p=lp->head->next;		//initialize *p at first value in list
	array=malloc(sizeof(void*)*(lp->count));	//allocate memory for void * pointers
	assert(array != NULL);
	for (i=0; i<lp->count; i++) {			//fill array with data from list
		array[i]=p->data;
		p=p->next;
	}
	return array;
}
	
