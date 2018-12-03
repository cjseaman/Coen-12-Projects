/* 	Collin Seaman
	Atkinson COEN 12
	Project 2

	AVERAGE RUN TIMES
						UNIQUE	PARITY
	Bible.txt 			-	0.467s	5.712s
	Genesis.txt			-	0.030s	0.088s
	GreenEggsAndHam.txt		-	0.009s	0.007s
	HoundOfTheBaskervilles.txt	-	0.062s	0.301s	
	Macbeth.txt			-	0.028s	0.077s
	TheCountOfMonteCristo.txt	-	0.453s	5.891s
	TheSecretGarden.txt		-	0.063s	0.367s
	TheWarOfTheWorlds.txt		-	0.082s	0.355s
	TreasureIsland.txt		-	0.071s	0.365s
	TwentyThousandLeagues.txt	-	0.130s	0.740s
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"
struct set 
{
	int count;
	int length;
	char **data;
};

typedef struct set SET;

SET *createSet(int maxElts)	//	O(1)
{
	SET *sp;	//set pointer
	sp=malloc(sizeof(SET));	//allocate memory for SET (2 int and one pointer)
	assert(sp!=NULL);
	sp->count=0;	//number of elements
	sp->length=maxElts;		
	sp->data=malloc(sizeof(char*)*maxElts);	//allocate memory for char pointers to strings
	assert(sp->data!=NULL);
	return sp;	//return mem location
}

void destroySet(SET *sp)	//	O(n)
{
	int i;
	for(i=0;i<sp->count;i++)
	{
		free(sp->data[i]);	//free each string
	}
	free(sp->data);	//free pointer to string pointers
	free(sp);	//free set pointer
}

int search(SET *sp, char *elt, bool *found)	//	O(logn)
{
	int lo, hi, mid, diff;	//declare indexes 
	lo=0;
	hi=sp->count-1;
	while(lo<=hi)	//loop to search, terminates AFTER mid==lo==hi
	{
		mid=(lo+hi)/2;			//resets each loop
		diff=strcmp(elt, sp->data[mid]);	
		if(diff<0)			//if word lies to the left of mid
		{
			hi=mid-1;
		}
		else if(diff>0)			//if word lies to the right of mid
		{
			lo=mid+1;
		}
		else
		{
			*found=true;
			return mid;	//when mid==word, word has been found
		}
	}
	*found=false;	//nothing has been returned up to this point
	return lo;	//lo is where the word belongs
}

char *findElement(SET *sp, char *elt)	//	O(logn)
{
	bool found;
	int locn;	//declaration of bool and index of element
	locn=search(sp, elt, &found);	//returns index
	if(found==true)
	{
		return sp->data[locn];	//returns adress of string at adress returned by search
	}
	return NULL;	//returns NULL if not found==false
}

void removeElement(SET *sp, char *elt)	//	O(logn)
{
	int locn, i;	//initialize indexes
	bool found;	//initialize bool
	assert(sp!=NULL&&elt!=NULL);	//ensure pointers are valid
	locn=search(sp, elt, &found);		//locn is the index of the word
	if (found==true)	//if the search found the word
	{
		for(i=locn; i<(sp->count); i++)	//loop to shift elements down
		{
			sp->data[i]=sp->data[i+1];
		}
		sp->count--;	//decrement count to 'remove' last element
	}
}

int numElements(SET *sp)	//	O(1)
{
	assert(sp!=NULL);
	return sp->count;				//returns number
}

void addElement(SET *sp, char *elt)	//	O(1)
{
	bool found;	//initialize
	int locn;
	int i;
	locn=search(sp, elt, &found);	//return adress word goes, or the word if it exists already
	if(found==true)	//if word is already in set
	{
		return;
	}
	if(found==false)	//else
	{	
		for(i=++sp->count; i>locn; i--)		//traverse backwards shifting pointers up
		{
			sp->data[i]=sp->data[i-1];
		}
		sp->data[locn]=strdup(elt);	//replaces pointer in index locn with a pointer to elt
		assert(sp->data[locn]!=NULL);	//hopefully
	}
}

char **getElements(SET *sp)	//	O(n)
{
	int i;
	char **strings;	//pointer to char pointers
	strings=malloc(sizeof(char*)*(sp->count));	//allocate memory for string pointers
	assert(strings!=NULL);				
	for(i=0;i<sp->count;i++)	//loop to allocate memory for each string
	{
		strings[i]=sp->data[i];	//copies strings from data to strings
	}
	return strings;	//return pointer to array
}
