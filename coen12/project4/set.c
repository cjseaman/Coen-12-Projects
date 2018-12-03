/*
	Collin Seaman
	Atkinson COEN 12
	Project 4: Hashing with chaining
	
	This is an implementation of a hashing algorithm that uses linked lists rather than probing to handle collisions.
	A linked list allows us to fill the hash indefinitely, and hopefully find an empty slot much faster. 
	Our hash table can also be much smaller as the total size can be less than the number of elements being added.

	Complexity for many functions is described by O(a), a being the expected length of each linked list within each hash.
*/

#include "set.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define AVG_LENGTH 20	//Average length of linked lists (expected "a" value)

struct set {		//holds the universal values for our entire set
	int count;
	int length;
	LIST **setList;
	int (*compare)();
	unsigned (*hash)();
};

SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()) {	//creates a new empty set O(n)
	int i;
	SET *setInfo;	
	LIST **table;		//array of pointers to structs containing head, count, and compare pointers
	table = malloc(sizeof(LIST*)*maxElts/AVG_LENGTH);		
	assert(table != NULL);
	for(i = 0; i<maxElts/AVG_LENGTH; i++) {		//this allocates one dummmy node per slot in the list
		table[i] = createList(compare);
	}
	setInfo = malloc(sizeof(SET));
	assert(setInfo != NULL);		//fill setinfo
	setInfo->count = 0;
	setInfo->length = maxElts/AVG_LENGTH;
	setInfo->setList = table;
	setInfo->compare = compare;
	setInfo->hash = hash;
	return setInfo;
}

void destroySet(SET *sp) {	//removes entire set and its data O(n)
	int i;
	for(i = 0;i<sp->length;i++) {
		destroyList(sp->setList[i]);	//deallocates all nodes in list
	}
	free(sp->setList);		//frees pointer to array of node pointers
	free(sp);			//frees set
}

int numElements(SET *sp) {	//returns number of elements in set O(1)
	return sp->count;
}

void addElement(SET *sp, void *elt) {	//Adds element to the hash location specified by hash() 
	int i = sp->hash(elt)%sp->length;
	if(findItem(sp->setList[i], elt) == NULL) {	//if item is not in set already
		addLast(sp->setList[i], elt);		//allocate and add to end of linked list
		sp->count++;
	}
}

void removeElement(SET *sp, void *elt) {	//removes element from hash location O(a)
	int i = sp->hash(elt)%sp->length;
	int initCount = numItems(sp->setList[i]);	//keeps track of whether count is decrememented to tell
	removeItem(sp->setList[i], elt);		//whether something was deleted
	if(numItems(sp->setList[i]) != initCount) {	//if something was deleted:
		sp->count--;				//decrement sp->count
	}
}
	
void *findElement(SET *sp, void *elt) {		//finds an element in the array and returns the value if found and NULL if
	int i = sp->hash(elt)%sp->length;	//not found. O(a)
	return findItem(sp->setList[i], elt);
}
	
void *getElements(SET *sp) {	//returns an array of void pointers to all values in set
	int i, j, length;
	void **array;
	void **tempArray;
	array = malloc(sizeof(void*)*sp->count);
	assert(array != NULL);
	tempArray = malloc(sizeof(void*)*sp->count);
	assert(tempArray != NULL);
	for(i = 0; i<sp->length; i++) {		//loop to traverse entire set dummy by dummy
		length=numItems(sp->setList[i]);	//if list is empty, length is zero
		tempArray=getItems(sp->setList[i]);	//and this does nothing
		for(j = i; j<i+length; j++) {		//and this doesn't run
			tempArray[j]=array[i];		//otherwise the items in the list are stored in temparray
		}					//and then moved to array
	}
	free(tempArray);				//and then temparray is freed
	return array;					//and we return array
}
			
			
		
