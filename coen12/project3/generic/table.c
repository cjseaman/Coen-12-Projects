/* 	Collin Seaman
	Atkinson COEN 12
	Project 2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"

#define EMPTY 0
#define FILLED 1
#define DELETED 2

struct set	//initialize set 
{
	int count;	//number of elements
	int length;	//size of array
	void **data;	//pointer to array of strings
	char *flag;	//flags
	int (*compare)();	//pointer to compare function
	unsigned(*hash)();	//pointer to hash function
};

SET *createSet(int maxElts, int(*compare)(), unsigned(*hash)())	//Function to create set (returns pointer to set)	O(n)
{
	int i;
	SET *sp;	//set pointer
	sp=malloc(sizeof(SET));	//allocate memory for SET (2 int and one pointer)
	assert(sp!=NULL);
	
	sp->count=0;	//number of elements
	sp->length=maxElts;		
	sp->data=malloc(sizeof(void*)*maxElts);	//allocate memory for char pointers to strings
	assert(sp->data!=NULL);
	
	sp->flag=malloc(sizeof(char)*maxElts);	//allocate for flags
	assert(sp->flag!=NULL);
	
	for(i=0;i<maxElts;i++)	//initializa all flags to EMPTY
	{
		sp->flag[i]=EMPTY;
	}
	
	sp->compare=compare;
	sp->hash=hash;	
	return sp;	//return mem location
}

void destroySet(SET *sp)	//Deletes set at end of all operations	O(1)
{
	free(sp->flag);	//free flags
	free(sp->data);	//free pointer to string pointers
	free(sp);	//free set pointer
}

int search(SET *sp, char *elt, bool *found)	//Returns address of found element or an empty slot	O(1)-O(n)
{
	int probe=sp->hash(elt)%sp->length;	//set probe
	int deleted, i;	
	bool isDeleted=false;
	*found=false;
	for(i=0;i<sp->length;i++)		//loops until value is returned
	{
		if(sp->flag[probe]==EMPTY)	//if slot is empty
		{
			if(isDeleted==true)	//if something was deleted
			{
				*found=false;
				return deleted;	//return that 
			}
			else
			{ 
				*found=false;
				return probe;	//if nothing was deleted return empty slot
			}
		}
		else if(sp->flag[probe]==DELETED)	//if slot is deleted
		{
			if(isDeleted==false)	//if no previous deleted value
			{
				deleted=probe;		//save value
				isDeleted=true;		//signal something was deleted
			}
		}
		else if(sp->flag[probe]==FILLED)	//if full
		{
			if(sp->compare(sp->data[probe], elt)==0)	//if the element exists already
			{
				*found=true;			//found it!
				return probe;			//return that address
			}
		} 
		probe=(probe+1)%sp->length;		//otherwise just continue
	}
	*found=false;
}

void *findElement(SET *sp, void *elt)	//Uses search to return pointer to string if found	O(1)-O(n)
{
	bool found;
	int locn;	//declaration of bool and index of element
	locn=search(sp, elt, &found);	//assigns locn and checks for valid index
	if(found==true)
	{
		return sp->data[locn];	//returns adress of string at adress returned by search
	}
	return NULL;	//returns NULL if found==false
}

void removeElement(SET *sp, void *elt)	//Frees memory and sets flag for deleted element	O(1)-O(n)
{
	int locn;	//initialize index
	bool found;	//initialize bool
	assert(sp!=NULL&&elt!=NULL);	//ensure pointers are valid
	locn=search(sp, elt, &found);
	if (found==true)	//if the search found the word
	{
		sp->flag[locn]=DELETED;	//just set the flag
		sp->count--;		//decrement count
	}
}

int numElements(SET *sp)	//Returns sp->count	O(1)
{
	assert(sp!=NULL);
	return sp->count;	//returns number
}

void addElement(SET *sp, void *elt)	//Sets flag and allocates and sets pointer for new element in set	O(1)
{
	bool found;	//initialize
	int locn;
	locn=search(sp, elt, &found);	//assigns locn
	if(found==true)	//if word is already in set
	{
		return;
	}
	if(found==false)	//else
	{
		sp->flag[locn]=FILLED;	//sets flag to full
		sp->data[locn]=elt;	//allocate and point
		assert(sp->data[locn]!=NULL);	
		sp->count++;		//more things so increment count
	}
}

void *getElements(SET *sp)	//returns a pointer to an array of all strings in set	O(n)
{
	int i, j;
	char **strings;
	strings=malloc(sizeof(char*)*sp->count);	//allocate memory for char pointers
	assert(strings!=NULL);	
	for(i=0, j=0;i<sp->length;i++)	//loop to copy address for each string
	{
		if(sp->flag[i]==FILLED)
		{
			strings[j]=sp->data[i];	//copies strings from data to strings
			j++;
		}
	}
	return strings;	//return pointer to array
}
